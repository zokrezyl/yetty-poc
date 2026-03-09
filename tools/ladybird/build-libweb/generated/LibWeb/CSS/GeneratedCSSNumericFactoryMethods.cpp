
#include <AK/FlyString.h>
#include <LibJS/Runtime/VM.h>
#include <LibWeb/CSS/CSSUnitValue.h>
#include <LibWeb/CSS/GeneratedCSSNumericFactoryMethods.h>

namespace Web::CSS {

// https://drafts.css-houdini.org/css-typed-om-1/#numeric-factory
inline GC::Ref<CSSUnitValue> numeric_factory(JS::VM& vm, WebIDL::Double value, FlyString unit)
{
    // All of the above methods must, when called with a double value, return a new CSSUnitValue whose value internal
    // slot is set to value and whose unit internal slot is set to the name of the method as defined here.
    return CSSUnitValue::create(*vm.current_realm(), value, move(unit));
}

GC::Ref<CSSUnitValue> number(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "number"_fly_string);
}

GC::Ref<CSSUnitValue> percent(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "percent"_fly_string);
}


// <angle>

GC::Ref<CSSUnitValue> deg(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "deg"_fly_string);
}

GC::Ref<CSSUnitValue> grad(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "grad"_fly_string);
}

GC::Ref<CSSUnitValue> rad(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "rad"_fly_string);
}

GC::Ref<CSSUnitValue> turn(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "turn"_fly_string);
}

// <flex>

GC::Ref<CSSUnitValue> fr(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "fr"_fly_string);
}

// <frequency>

GC::Ref<CSSUnitValue> hz(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "Hz"_fly_string);
}

GC::Ref<CSSUnitValue> khz(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "kHz"_fly_string);
}

// <length>

GC::Ref<CSSUnitValue> cap(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "cap"_fly_string);
}

GC::Ref<CSSUnitValue> ch(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "ch"_fly_string);
}

GC::Ref<CSSUnitValue> cm(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "cm"_fly_string);
}

GC::Ref<CSSUnitValue> dvb(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "dvb"_fly_string);
}

GC::Ref<CSSUnitValue> dvh(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "dvh"_fly_string);
}

GC::Ref<CSSUnitValue> dvi(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "dvi"_fly_string);
}

GC::Ref<CSSUnitValue> dvmax(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "dvmax"_fly_string);
}

GC::Ref<CSSUnitValue> dvmin(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "dvmin"_fly_string);
}

GC::Ref<CSSUnitValue> dvw(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "dvw"_fly_string);
}

GC::Ref<CSSUnitValue> em(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "em"_fly_string);
}

GC::Ref<CSSUnitValue> ex(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "ex"_fly_string);
}

GC::Ref<CSSUnitValue> ic(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "ic"_fly_string);
}

GC::Ref<CSSUnitValue> in(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "in"_fly_string);
}

GC::Ref<CSSUnitValue> lh(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "lh"_fly_string);
}

GC::Ref<CSSUnitValue> lvb(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "lvb"_fly_string);
}

GC::Ref<CSSUnitValue> lvh(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "lvh"_fly_string);
}

GC::Ref<CSSUnitValue> lvi(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "lvi"_fly_string);
}

GC::Ref<CSSUnitValue> lvmax(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "lvmax"_fly_string);
}

GC::Ref<CSSUnitValue> lvmin(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "lvmin"_fly_string);
}

GC::Ref<CSSUnitValue> lvw(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "lvw"_fly_string);
}

GC::Ref<CSSUnitValue> mm(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "mm"_fly_string);
}

GC::Ref<CSSUnitValue> pc(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "pc"_fly_string);
}

GC::Ref<CSSUnitValue> pt(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "pt"_fly_string);
}

GC::Ref<CSSUnitValue> px(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "px"_fly_string);
}

GC::Ref<CSSUnitValue> q(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "Q"_fly_string);
}

GC::Ref<CSSUnitValue> rcap(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "rcap"_fly_string);
}

GC::Ref<CSSUnitValue> rch(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "rch"_fly_string);
}

GC::Ref<CSSUnitValue> rem(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "rem"_fly_string);
}

GC::Ref<CSSUnitValue> rex(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "rex"_fly_string);
}

GC::Ref<CSSUnitValue> ric(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "ric"_fly_string);
}

GC::Ref<CSSUnitValue> rlh(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "rlh"_fly_string);
}

GC::Ref<CSSUnitValue> svb(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "svb"_fly_string);
}

GC::Ref<CSSUnitValue> svh(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "svh"_fly_string);
}

GC::Ref<CSSUnitValue> svi(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "svi"_fly_string);
}

GC::Ref<CSSUnitValue> svmax(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "svmax"_fly_string);
}

GC::Ref<CSSUnitValue> svmin(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "svmin"_fly_string);
}

GC::Ref<CSSUnitValue> svw(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "svw"_fly_string);
}

GC::Ref<CSSUnitValue> vb(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "vb"_fly_string);
}

GC::Ref<CSSUnitValue> vh(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "vh"_fly_string);
}

GC::Ref<CSSUnitValue> vi(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "vi"_fly_string);
}

GC::Ref<CSSUnitValue> vmax(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "vmax"_fly_string);
}

GC::Ref<CSSUnitValue> vmin(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "vmin"_fly_string);
}

GC::Ref<CSSUnitValue> vw(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "vw"_fly_string);
}

// <resolution>

GC::Ref<CSSUnitValue> dpcm(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "dpcm"_fly_string);
}

GC::Ref<CSSUnitValue> dpi(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "dpi"_fly_string);
}

GC::Ref<CSSUnitValue> dppx(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "dppx"_fly_string);
}

GC::Ref<CSSUnitValue> x(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "x"_fly_string);
}

// <time>

GC::Ref<CSSUnitValue> ms(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "ms"_fly_string);
}

GC::Ref<CSSUnitValue> s(JS::VM& vm, WebIDL::Double value)
{
    return numeric_factory(vm, value, "s"_fly_string);
}

}
