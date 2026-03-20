#pragma once

#include "yrich-operation.h"
#include <msgpack.hpp>
#include <sstream>

namespace yetty::yrich {

//=============================================================================
// Msgpack adapters for YRich types
//=============================================================================

// Timestamp
inline void pack_timestamp(msgpack::packer<std::stringstream>& pk, const Timestamp& ts) {
    pk.pack_array(2);
    pk.pack(ts.counter);
    pk.pack(ts.session);
}

inline Timestamp unpack_timestamp(const msgpack::object& obj) {
    if (obj.type != msgpack::type::ARRAY || obj.via.array.size != 2) {
        return {};
    }
    Timestamp ts;
    ts.counter = obj.via.array.ptr[0].as<uint64_t>();
    ts.session = obj.via.array.ptr[1].as<uint64_t>();
    return ts;
}

// CellAddress
inline void pack_cell_address(msgpack::packer<std::stringstream>& pk, const CellAddress& addr) {
    pk.pack_array(2);
    pk.pack(addr.row);
    pk.pack(addr.col);
}

inline CellAddress unpack_cell_address(const msgpack::object& obj) {
    if (obj.type != msgpack::type::ARRAY || obj.via.array.size != 2) {
        return {};
    }
    CellAddress addr;
    addr.row = obj.via.array.ptr[0].as<int32_t>();
    addr.col = obj.via.array.ptr[1].as<int32_t>();
    return addr;
}

// CellRange
inline void pack_cell_range(msgpack::packer<std::stringstream>& pk, const CellRange& range) {
    pk.pack_array(2);
    pack_cell_address(pk, range.start);
    pack_cell_address(pk, range.end);
}

inline CellRange unpack_cell_range(const msgpack::object& obj) {
    if (obj.type != msgpack::type::ARRAY || obj.via.array.size != 2) {
        return {};
    }
    CellRange range;
    range.start = unpack_cell_address(obj.via.array.ptr[0]);
    range.end = unpack_cell_address(obj.via.array.ptr[1]);
    return range;
}

//=============================================================================
// Operation serialization
//=============================================================================

class OperationSerializer {
public:
    // Serialize operation to bytes
    static std::vector<uint8_t> serialize(const Operation& op) {
        std::stringstream ss;
        msgpack::packer<std::stringstream> pk(ss);

        // Pack header: [type, timestamp, author]
        pk.pack_array(4);
        pk.pack(static_cast<int>(op.type()));
        pack_timestamp(pk, op.timestamp());
        pk.pack(op.author());

        // Pack data based on type
        packOperationData(pk, op.type(), op.data());

        std::string str = ss.str();
        return std::vector<uint8_t>(str.begin(), str.end());
    }

    // Serialize multiple operations
    static std::vector<uint8_t> serializeMany(const std::vector<OperationPtr>& ops) {
        std::stringstream ss;
        msgpack::packer<std::stringstream> pk(ss);

        pk.pack_array(ops.size());
        for (const auto& op : ops) {
            auto bytes = serialize(*op);
            pk.pack_bin(bytes.size());
            pk.pack_bin_body(reinterpret_cast<const char*>(bytes.data()), bytes.size());
        }

        std::string str = ss.str();
        return std::vector<uint8_t>(str.begin(), str.end());
    }

    // Deserialize operation from bytes
    static OperationPtr deserialize(const uint8_t* data, size_t size) {
        try {
            msgpack::object_handle oh = msgpack::unpack(
                reinterpret_cast<const char*>(data), size);
            msgpack::object obj = oh.get();

            if (obj.type != msgpack::type::ARRAY || obj.via.array.size != 4) {
                return nullptr;
            }

            auto type = static_cast<OperationType>(obj.via.array.ptr[0].as<int>());
            auto timestamp = unpack_timestamp(obj.via.array.ptr[1]);
            auto author = obj.via.array.ptr[2].as<SessionId>();
            auto opData = unpackOperationData(type, obj.via.array.ptr[3]);

            return std::make_shared<Operation>(type, std::move(opData), timestamp, author);
        } catch (...) {
            return nullptr;
        }
    }

    // Deserialize multiple operations
    static std::vector<OperationPtr> deserializeMany(const uint8_t* data, size_t size) {
        std::vector<OperationPtr> result;
        try {
            msgpack::object_handle oh = msgpack::unpack(
                reinterpret_cast<const char*>(data), size);
            msgpack::object obj = oh.get();

            if (obj.type != msgpack::type::ARRAY) {
                return result;
            }

            for (size_t i = 0; i < obj.via.array.size; ++i) {
                auto& item = obj.via.array.ptr[i];
                if (item.type == msgpack::type::BIN) {
                    auto op = deserialize(
                        reinterpret_cast<const uint8_t*>(item.via.bin.ptr),
                        item.via.bin.size);
                    if (op) result.push_back(op);
                }
            }
        } catch (...) {
        }
        return result;
    }

private:
    static void packOperationData(msgpack::packer<std::stringstream>& pk,
                                   OperationType type, const OperationData& data) {
        switch (type) {
            case OperationType::Insert: {
                auto& d = std::get<InsertOp>(data);
                pk.pack_array(3);
                pk.pack(d.id);
                pk.pack(d.elementType);
                pk.pack(d.data);
                break;
            }
            case OperationType::Delete: {
                auto& d = std::get<DeleteOp>(data);
                pk.pack_array(2);
                pk.pack(d.id);
                pk.pack(d.data);
                break;
            }
            case OperationType::Update: {
                auto& d = std::get<UpdateOp>(data);
                pk.pack_array(4);
                pk.pack(d.id);
                pk.pack(d.property);
                pk.pack(d.oldValue);
                pk.pack(d.newValue);
                break;
            }
            case OperationType::TextInsert: {
                auto& d = std::get<TextInsertOp>(data);
                pk.pack_array(3);
                pk.pack(d.id);
                pk.pack(d.position);
                pk.pack(d.text);
                break;
            }
            case OperationType::TextDelete: {
                auto& d = std::get<TextDeleteOp>(data);
                pk.pack_array(4);
                pk.pack(d.id);
                pk.pack(d.position);
                pk.pack(d.length);
                pk.pack(d.deletedText);
                break;
            }
            case OperationType::CellSet: {
                auto& d = std::get<CellSetOp>(data);
                pk.pack_array(3);
                pack_cell_address(pk, d.addr);
                pk.pack(d.oldValue);
                pk.pack(d.newValue);
                break;
            }
            case OperationType::CellClear: {
                auto& d = std::get<CellClearOp>(data);
                pk.pack_array(2);
                pack_cell_range(pk, d.range);
                pk.pack_array(d.oldValues.size());
                for (const auto& [addr, val] : d.oldValues) {
                    pk.pack_array(2);
                    pack_cell_address(pk, addr);
                    pk.pack(val);
                }
                break;
            }
            case OperationType::TextFormat: {
                auto& d = std::get<TextFormatOp>(data);
                pk.pack_array(6);
                pk.pack(d.id);
                pk.pack(d.startPos);
                pk.pack(d.endPos);
                pk.pack(d.addFormat);
                pk.pack(d.removeFormat);
                pk.pack(d.oldFormat);
                break;
            }
            case OperationType::CellFormat: {
                auto& d = std::get<CellFormatOp>(data);
                pk.pack_array(4);
                pack_cell_range(pk, d.range);
                pk.pack(d.property);
                pk.pack(d.newValue);
                pk.pack_array(d.oldValues.size());
                for (const auto& [addr, val] : d.oldValues) {
                    pk.pack_array(2);
                    pack_cell_address(pk, addr);
                    pk.pack(val);
                }
                break;
            }
            case OperationType::StyleSet: {
                auto& d = std::get<StyleSetOp>(data);
                pk.pack_array(4);
                pk.pack(d.id);
                pk.pack(d.property);
                pk.pack(d.oldValue);
                pk.pack(d.newValue);
                break;
            }
            case OperationType::CursorMove: {
                auto& d = std::get<CursorMoveOp>(data);
                pk.pack_array(2);
                pk.pack(d.elementId);
                pk.pack(d.position);
                break;
            }
            case OperationType::SelectionSet: {
                // Selection serialization - simplified for now
                pk.pack_nil();
                break;
            }
            default:
                pk.pack_nil();
                break;
        }
    }

    static OperationData unpackOperationData(OperationType type, const msgpack::object& obj) {
        switch (type) {
            case OperationType::Insert: {
                InsertOp d;
                if (obj.type == msgpack::type::ARRAY && obj.via.array.size >= 3) {
                    d.id = obj.via.array.ptr[0].as<ElementId>();
                    d.elementType = obj.via.array.ptr[1].as<std::string>();
                    d.data = obj.via.array.ptr[2].as<std::string>();
                }
                return d;
            }
            case OperationType::Delete: {
                DeleteOp d;
                if (obj.type == msgpack::type::ARRAY && obj.via.array.size >= 2) {
                    d.id = obj.via.array.ptr[0].as<ElementId>();
                    d.data = obj.via.array.ptr[1].as<std::string>();
                }
                return d;
            }
            case OperationType::Update: {
                UpdateOp d;
                if (obj.type == msgpack::type::ARRAY && obj.via.array.size >= 4) {
                    d.id = obj.via.array.ptr[0].as<ElementId>();
                    d.property = obj.via.array.ptr[1].as<std::string>();
                    d.oldValue = obj.via.array.ptr[2].as<std::string>();
                    d.newValue = obj.via.array.ptr[3].as<std::string>();
                }
                return d;
            }
            case OperationType::TextInsert: {
                TextInsertOp d;
                if (obj.type == msgpack::type::ARRAY && obj.via.array.size >= 3) {
                    d.id = obj.via.array.ptr[0].as<ElementId>();
                    d.position = obj.via.array.ptr[1].as<int>();
                    d.text = obj.via.array.ptr[2].as<std::string>();
                }
                return d;
            }
            case OperationType::TextDelete: {
                TextDeleteOp d;
                if (obj.type == msgpack::type::ARRAY && obj.via.array.size >= 4) {
                    d.id = obj.via.array.ptr[0].as<ElementId>();
                    d.position = obj.via.array.ptr[1].as<int>();
                    d.length = obj.via.array.ptr[2].as<int>();
                    d.deletedText = obj.via.array.ptr[3].as<std::string>();
                }
                return d;
            }
            case OperationType::CellSet: {
                CellSetOp d;
                if (obj.type == msgpack::type::ARRAY && obj.via.array.size >= 3) {
                    d.addr = unpack_cell_address(obj.via.array.ptr[0]);
                    d.oldValue = obj.via.array.ptr[1].as<std::string>();
                    d.newValue = obj.via.array.ptr[2].as<std::string>();
                }
                return d;
            }
            case OperationType::CellClear: {
                CellClearOp d;
                if (obj.type == msgpack::type::ARRAY && obj.via.array.size >= 2) {
                    d.range = unpack_cell_range(obj.via.array.ptr[0]);
                    auto& arr = obj.via.array.ptr[1];
                    if (arr.type == msgpack::type::ARRAY) {
                        for (size_t i = 0; i < arr.via.array.size; ++i) {
                            auto& item = arr.via.array.ptr[i];
                            if (item.type == msgpack::type::ARRAY && item.via.array.size >= 2) {
                                auto addr = unpack_cell_address(item.via.array.ptr[0]);
                                auto val = item.via.array.ptr[1].as<std::string>();
                                d.oldValues.push_back({addr, val});
                            }
                        }
                    }
                }
                return d;
            }
            case OperationType::TextFormat: {
                TextFormatOp d;
                if (obj.type == msgpack::type::ARRAY && obj.via.array.size >= 6) {
                    d.id = obj.via.array.ptr[0].as<ElementId>();
                    d.startPos = obj.via.array.ptr[1].as<int>();
                    d.endPos = obj.via.array.ptr[2].as<int>();
                    d.addFormat = obj.via.array.ptr[3].as<uint32_t>();
                    d.removeFormat = obj.via.array.ptr[4].as<uint32_t>();
                    d.oldFormat = obj.via.array.ptr[5].as<uint32_t>();
                }
                return d;
            }
            case OperationType::CellFormat: {
                CellFormatOp d;
                if (obj.type == msgpack::type::ARRAY && obj.via.array.size >= 4) {
                    d.range = unpack_cell_range(obj.via.array.ptr[0]);
                    d.property = obj.via.array.ptr[1].as<std::string>();
                    d.newValue = obj.via.array.ptr[2].as<std::string>();
                    auto& arr = obj.via.array.ptr[3];
                    if (arr.type == msgpack::type::ARRAY) {
                        for (size_t i = 0; i < arr.via.array.size; ++i) {
                            auto& item = arr.via.array.ptr[i];
                            if (item.type == msgpack::type::ARRAY && item.via.array.size >= 2) {
                                auto addr = unpack_cell_address(item.via.array.ptr[0]);
                                auto val = item.via.array.ptr[1].as<std::string>();
                                d.oldValues.push_back({addr, val});
                            }
                        }
                    }
                }
                return d;
            }
            case OperationType::StyleSet: {
                StyleSetOp d;
                if (obj.type == msgpack::type::ARRAY && obj.via.array.size >= 4) {
                    d.id = obj.via.array.ptr[0].as<ElementId>();
                    d.property = obj.via.array.ptr[1].as<std::string>();
                    d.oldValue = obj.via.array.ptr[2].as<std::string>();
                    d.newValue = obj.via.array.ptr[3].as<std::string>();
                }
                return d;
            }
            case OperationType::CursorMove: {
                CursorMoveOp d;
                if (obj.type == msgpack::type::ARRAY && obj.via.array.size >= 2) {
                    d.elementId = obj.via.array.ptr[0].as<ElementId>();
                    d.position = obj.via.array.ptr[1].as<int>();
                }
                return d;
            }
            default:
                return CursorMoveOp{};
        }
    }
};

} // namespace yetty::yrich
