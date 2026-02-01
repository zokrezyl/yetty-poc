"""
Result type for yetty card Python scripts.

Provides Rust-like Result[T] pattern with Ok/Err/Error for structured
error handling between Python scripts and the C++ card runtime.

Usage in card scripts:
    from result import Result, Ok, Err

    def init(width_cells, height_cells):
        try:
            do_stuff()
        except Exception as e:
            return Result.error("init failed", e)
        return Ok(None)

    def render(time):
        if not _ready:
            return Result.error("not ready")
        do_render()
        return Ok(None)
"""

import traceback
from dataclasses import dataclass
from typing import Callable, Generic, Optional, TypeVar, Union, Any
from abc import abstractmethod


def as_tree(obj: Any) -> Any:
    """Convert an object to a serializable tree structure."""
    if hasattr(obj, "as_tree"):
        return obj.as_tree
    if isinstance(obj, dict):
        return {k: as_tree(v) for k, v in obj.items()}
    if isinstance(obj, list):
        return [as_tree(v) for v in obj]
    if isinstance(obj, (int, float, str, bool)):
        return obj
    if obj is None:
        return None
    return str(obj)


class Error:
    """Inner representation of an error with optional chaining."""

    def __init__(self, error, prev_error=None):
        self._error = error
        self._prev_error = prev_error
        self._stack = traceback.extract_stack()[:-1]

    @classmethod
    def create(cls, value, prev_error=None):
        """Create an Error, adapting exceptions and other types."""
        return cls(_adapt_error(value), _adapt_error(prev_error))

    @property
    def error(self):
        return self._error

    @property
    def prev_error(self):
        return self._prev_error

    @property
    def stack(self):
        return self._stack

    @property
    def message(self) -> str:
        """Flat string representation for C++ error chaining."""
        parts = []
        if isinstance(self._error, str):
            parts.append(self._error)
        elif isinstance(self._error, dict):
            parts.append(self._error.get("message", str(self._error)))
        else:
            parts.append(str(self._error))
        if self._prev_error is not None:
            if isinstance(self._prev_error, Error):
                parts.append(self._prev_error.message)
            elif isinstance(self._prev_error, str):
                parts.append(self._prev_error)
            elif isinstance(self._prev_error, dict):
                parts.append(self._prev_error.get("message", str(self._prev_error)))
            else:
                parts.append(str(self._prev_error))
        return " <- ".join(parts)

    @property
    def as_tree(self) -> dict:
        return {
            "error": as_tree(self._error),
            "prev_error": as_tree(self._prev_error),
        }

    def __repr__(self):
        return f"Error({self._error!r})"

    def __str__(self):
        return self.message


def _adapt_error(value):
    """Convert any error-like value to a normalized form."""
    if value is None:
        return None
    if isinstance(value, Error):
        return value
    if isinstance(value, str):
        return value
    if isinstance(value, dict):
        return value
    if isinstance(value, Exception):
        stack = traceback.format_tb(value.__traceback__) if value.__traceback__ else None
        return {
            "type": type(value).__name__,
            "message": str(value),
            "module": getattr(type(value), "__module__", None),
            "stack": stack,
        }
    if isinstance(value, Err):
        return value._error
    return {"type": type(value).__name__, "value": str(value)}


T = TypeVar("T")
U = TypeVar("U")


class Result(Generic[T]):
    """Base class for Ok and Err."""

    def __bool__(self) -> bool:
        return self.is_ok

    @property
    @abstractmethod
    def is_ok(self) -> bool: ...

    @property
    @abstractmethod
    def is_err(self) -> bool: ...

    @property
    @abstractmethod
    def unwrapped(self) -> T: ...

    @classmethod
    def error(cls, err, prev_error=None) -> "Result[T]":
        """Create an Err result. Accepts strings, exceptions, dicts, other Results."""
        return Err.create(err, prev_error)

    def and_then(self, func: Callable[[T], "Result[U]"]) -> "Result[U]":
        if self.is_ok:
            return func(self.unwrapped)
        return self  # type: ignore

    def map(self, func: Callable[[T], U]) -> "Result[U]":
        if self.is_ok:
            return Ok(func(self.unwrapped))
        return self  # type: ignore

    def unwrap_or(self, default: T) -> T:
        return self.unwrapped if self.is_ok else default


@dataclass(frozen=True)
class Ok(Result[T]):
    _value: T

    @property
    def is_ok(self) -> bool:
        return True

    @property
    def is_err(self) -> bool:
        return False

    @property
    def unwrapped(self) -> T:
        return self._value

    @property
    def as_tree(self) -> dict:
        return {"value": as_tree(self._value)}


@dataclass(frozen=True)
class Err(Result[T]):
    _error: Error

    def __post_init__(self):
        if not isinstance(self._error, Error):
            object.__setattr__(self, '_error', Error.create(self._error))

    @property
    def is_ok(self) -> bool:
        return False

    @property
    def is_err(self) -> bool:
        return True

    @property
    def error(self) -> Error:
        return self._error

    @property
    def unwrapped(self) -> T:
        raise RuntimeError(f"Cannot unwrap Err: {self._error}")

    @property
    def as_tree(self) -> dict:
        return {"error": self._error.as_tree}

    @classmethod
    def create(cls, error, prev_error=None) -> "Err[T]":
        return cls(_error=Error.create(error, prev_error))

    def __repr__(self) -> str:
        return f"Err({self._error!r})"
