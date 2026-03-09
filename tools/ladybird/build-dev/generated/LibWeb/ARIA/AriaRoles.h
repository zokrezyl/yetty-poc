
#pragma once

#include <LibWeb/ARIA/RoleType.h>

namespace Web::ARIA {

// https://www.w3.org/TR/wai-aria-1.2/#structure
// A document structural element.
class Structure :
 public RoleType
{
public:
    Structure(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Structure();
};

// https://www.w3.org/TR/wai-aria-1.2/#widget
// An interactive component of a graphical user interface (GUI).
class Widget :
 public RoleType
{
public:
    Widget(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Widget();
};

// https://www.w3.org/TR/wai-aria-1.2/#window
// A browser or application window.
class Window :
 public RoleType
{
public:
    Window(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Window();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#composite
// A widget that may contain navigable descendants or owned children.
class Composite :
 public Widget
{
public:
    Composite(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Composite();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#application
// A structure containing one or more focusable elements requiring user input, such as keyboard or gesture events, that do not follow a standard interaction pattern supported by a widget role.
class Application :
 public Structure
{
public:
    Application(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Application();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#document
// An element containing content that assistive technology users may want to browse in a reading mode.
class Document :
 public Structure
{
public:
    Document(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Document();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#generic
// A nameless container element that has no semantic meaning on its own.
class Generic :
 public Structure
{
public:
    Generic(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Generic();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#presentation
// An element whose implicit native role semantics will not be mapped to the accessibility API. See synonym none.
class Presentation :
 public Structure
{
public:
    Presentation(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Presentation();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#range
// An element representing a range of values.
class Range :
 public Structure
{
public:
    Range(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Range();
};

// https://www.w3.org/TR/wai-aria-1.2/#rowgroup
// A structure containing one or more row elements in a tabular container.
class RowGroup :
 public Structure
{
public:
    RowGroup(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    RowGroup();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#section
// A renderable structural containment unit in a document or application.
class Section :
 public Structure
{
public:
    Section(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Section();
};

// https://w3c.github.io/aria/#sectionfooter
// A set of user interface objects and information representing information about its closest ancestral content group.
class SectionFooter :
 public Section
{
public:
    SectionFooter(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    SectionFooter();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://w3c.github.io/aria/#sectionheader
// A set of user interface objects and information that represents a collection of introductory items for the element's closest ancestral content group.
class SectionHeader :
 public Section
{
public:
    SectionHeader(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    SectionHeader();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#input
// A generic type of widget that allows user input.
class Input :
 public Widget
{
public:
    Input(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Input();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#sectionhead
// A structure that labels or summarizes the topic of its related section.
class SectionHead :
 public Structure
{
public:
    SectionHead(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    SectionHead();
};

// https://www.w3.org/TR/wai-aria-1.2/#separator
// A divider that separates and distinguishes sections of content or groups of menuitems.
// NOTE: This is not an actual aria role, but Separator has distinct behavior and inheritance
//       depending on if it is focusable or not, so it is implemented as two classes.
//       Also see NonFocusableSeparator.
class FocusableSeparator :
 public Widget
{
public:
    FocusableSeparator(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    FocusableSeparator();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#separator
// A divider that separates and distinguishes sections of content or groups of menuitems.
// NOTE: This is not an actual aria role, but Separator has distinct behavior and inheritance
//       depending on if it is focusable or not, so it is implemented as two classes.
//       Also see FocusableSeparator.
class NonFocusableSeparator :
 public Structure
{
public:
    NonFocusableSeparator(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    NonFocusableSeparator();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#article
// A section of a page that consists of a composition that forms an independent part of a document, page, or site.
class Article :
 public Document
{
public:
    Article(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Article();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#meter
// An element that represents a scalar measurement within a known range, or a fractional value. See related progressbar.
class Meter :
 public Range
{
public:
    Meter(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Meter();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#progressbar
// An element that displays the progress status for tasks that take a long time.
// NOTE: Progresbar also inherits from Widgetfrom Cell, but we can't do that in C++ due to Progressbar already inheriting from Widget
//       This is fine because behavior is still correct and is() will still work as expected.
class Progressbar :
 public Range
{
public:
    Progressbar(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Progressbar();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#scrollbar
// A graphical object that controls the scrolling of content within a viewing area, regardless of whether the content is fully displayed within the viewing area.
class Scrollbar :
 public Range, public Widget
{
public:
    Scrollbar(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Scrollbar();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#slider
// An input where the user selects a value from within a given range.
class Slider :
 public Input, public Range
{
public:
    Slider(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Slider();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#spinbutton
// A form of range that expects the user to select from among discrete choices.
class SpinButton :
 public Composite, public Input, public Range
{
public:
    SpinButton(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    SpinButton();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#alert
// A type of live region with important, and usually time-sensitive, information. See related alertdialog and status.
class Alert :
 public Section
{
public:
    Alert(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Alert();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#blockquote
// A section of content that is quoted from another source.
class BlockQuote :
 public Section
{
public:
    BlockQuote(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    BlockQuote();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#caption
// Visible content that names, and may also describe, a figure, table, grid, or treegrid.
class Caption :
 public Section
{
public:
    Caption(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Caption();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#cell
// A cell in a tabular container. See related gridcell.
class Cell :
 public Section
{
public:
    Cell(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Cell();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#code
// A section whose content represents a fragment of computer code.
class Code :
 public Section
{
public:
    Code(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Code();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#definition
// A definition of a term or concept. See related term.
class Definition :
 public Section
{
public:
    Definition(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Definition();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#deletion
// A deletion contains content that is marked as removed or content that is being suggested for removal. See related insertion.
class Deletion :
 public Section
{
public:
    Deletion(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Deletion();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#emphasis
// One or more emphasized characters. See related strong.
class Emphasis :
 public Section
{
public:
    Emphasis(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Emphasis();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#figure
// A perceivable section of content that typically contains a graphical document, images, code snippets, or example text. The parts of a figure MAY be user-navigable.
class Figure :
 public Section
{
public:
    Figure(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Figure();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#group
// A set of user interface objects that is not intended to be included in a page summary or table of contents by assistive technologies.
class Group :
 public Section
{
public:
    Group(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Group();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#img
// A container for a collection of elements that form an image.
class Img :
 public Section
{
public:
    Img(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Img();

public:
    virtual NameFromSource name_from_source() const override;
};

// 
// 
class Insertion :
 public Section
{
public:
    Insertion(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Insertion();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#landmark
// A perceivable section containing content that is relevant to a specific, author-specified purpose and sufficiently important that users will likely want to be able to navigate to the section easily and to have it listed in a summary of the page. Such a page summary could be generated dynamically by a user agent or assistive technology.
class Landmark :
 public Section
{
public:
    Landmark(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Landmark();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#list
// A section containing listitem elements. See related listbox.
class List :
 public Section
{
public:
    List(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    List();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#listitem
// A single item in a list or directory.
class ListItem :
 public Section
{
public:
    ListItem(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    ListItem();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#log
// A type of live region where new information is added in meaningful order and old information may disappear. See related marquee.
class Log :
 public Section
{
public:
    Log(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Log();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#marquee
// A type of live region where non-essential information changes frequently. See related log.
class Marquee :
 public Section
{
public:
    Marquee(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Marquee();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#math
// Content that represents a mathematical expression.
class Math :
 public Section
{
public:
    Math(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Math();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#note
// A section whose content is parenthetic or ancillary to the main content of the resource.
class Note :
 public Section
{
public:
    Note(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Note();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#paragraph
// A paragraph of content.
class Paragraph :
 public Section
{
public:
    Paragraph(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Paragraph();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#status
// A type of live region whose content is advisory information for the user but is not important enough to justify an alert, often but not necessarily presented as a status bar.
class Status :
 public Section
{
public:
    Status(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Status();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://w3c.github.io/aria/#strong
// Content that is important, serious, or urgent.
class Strong :
 public Section
{
public:
    Strong(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Strong();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#subscript
// One or more subscripted characters. See related superscript.
class Subscript :
 public Section
{
public:
    Subscript(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Subscript();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://w3c.github.io/aria/#suggestion
// A single proposed change to content.
class Suggestion :
 public Section
{
public:
    Suggestion(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Suggestion();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#superscript
// One or more superscripted characters. See related superscript.
class Superscript :
 public Section
{
public:
    Superscript(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Superscript();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#table
// A section containing data arranged in rows and columns. See related grid.
class Table :
 public Section
{
public:
    Table(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Table();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#tabpanel
// A container for the resources associated with a tab, where each tab is contained in a tablist.
class TabPanel :
 public Section
{
public:
    TabPanel(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    TabPanel();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#term
// A word or phrase with a corresponding definition. See related definition.
class Term :
 public Section
{
public:
    Term(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Term();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#time
// An element that represents a specific point in time.
class Time :
 public Section
{
public:
    Time(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Time();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#tooltip
// A contextual popup that displays a description for an element.
class Tooltip :
 public Section
{
public:
    Tooltip(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Tooltip();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#dialog
// A dialog is a descendant window of the primary window of a web application. For HTML pages, the primary application window is the entire web document, i.e., the body element.
class Dialog :
 public Window
{
public:
    Dialog(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Dialog();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#alertdialog
// A type of dialog that contains an alert message, where initial focus goes to an element within the dialog. See related alert and dialog.
class AlertDialog :
 public Alert, public Dialog
{
public:
    AlertDialog(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    AlertDialog();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#gridcell
// A cell in a grid or treegrid.
class GridCell :
 public Cell, public Widget
{
public:
    GridCell(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    GridCell();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#columnheader
// A cell containing header information for a column.
// NOTE: ColumnHeader also inherits from Cell, but we can't do that in C++ due to GridCell already inheriting from Cell
//       This is fine because behavior is still correct and is() will still work as expected.
class ColumnHeader :
 public GridCell, public SectionHead
{
public:
    ColumnHeader(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    ColumnHeader();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#rowheader
// A cell containing header information for a row.
// NOTE: RowHeader also inherits from Cell, but we can't do that in C++ due to GridCell already inheriting from Cell
//       This is fine because behavior is still correct and is() will still work as expected.
class RowHeader :
 public GridCell, public SectionHead
{
public:
    RowHeader(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    RowHeader();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#row
// A row of cells in a tabular container.
class Row :
 public Group, public Widget
{
public:
    Row(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Row();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#select
// A form widget that allows the user to make selections from a set of choices.
class Select :
 public Composite, public Group
{
public:
    Select(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Select();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#toolbar
// A collection of commonly used function buttons or controls represented in compact visual form.
class Toolbar :
 public Group
{
public:
    Toolbar(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Toolbar();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#listbox
// A widget that allows the user to select one or more items from a list of choices. See related combobox and list.
class ListBox :
 public Select
{
public:
    ListBox(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    ListBox();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#menu
// A type of widget that offers a list of choices to the user.
class Menu :
 public Select
{
public:
    Menu(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Menu();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#radiogroup
// A group of radio buttons.
class RadioGroup :
 public Select
{
public:
    RadioGroup(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    RadioGroup();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#tree
// A widget that allows the user to select one or more items from a hierarchically organized collection.
class Tree :
 public Select
{
public:
    Tree(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Tree();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#menubar
// A presentation of menu that usually remains visible and is usually presented horizontally.
class MenuBar :
 public Menu
{
public:
    MenuBar(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    MenuBar();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#treegrid
// A grid whose rows can be expanded and collapsed in the same manner as for a tree.
class TreeGrid :
 public Row
{
public:
    TreeGrid(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    TreeGrid();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#banner
// https://www.w3.org/TR/wai-aria-1.2/#banner
class Banner :
 public Landmark
{
public:
    Banner(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Banner();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#complementary
// A landmark that is designed to be complementary to the main content at a similar level in the DOM hierarchy, but remaining meaningful when separated from the main content.
class Complementary :
 public Landmark
{
public:
    Complementary(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Complementary();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#contentinfo
// A landmark that contains information about the parent document.
class ContentInfo :
 public Landmark
{
public:
    ContentInfo(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    ContentInfo();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#form
// A landmark region that contains a collection of items and objects that, as a whole, combine to create a form. See related search.
class Form :
 public Landmark
{
public:
    Form(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Form();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#main
// A landmark containing the main content of a document.
class Main :
 public Landmark
{
public:
    Main(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Main();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://w3c.github.io/aria/#mark
// Content which is marked or highlighted for reference or notation purposes, due to the content's relevance in the enclosing context.
class Mark :
 public Section
{
public:
    Mark(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Mark();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#navigation
// A landmark containing a collection of navigational elements (usually links) for navigating the document or related documents.
class Navigation :
 public Landmark
{
public:
    Navigation(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Navigation();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#region
// A landmark containing content that is relevant to a specific, author-specified purpose and sufficiently important that users will likely want to be able to navigate to the section easily and to have it listed in a summary of the page. Such a page summary could be generated dynamically by a user agent or assistive technology.
class Region :
 public Landmark
{
public:
    Region(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Region();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#search
// A landmark region that contains a collection of items and objects that, as a whole, combine to create a search facility. See related form and searchbox.
class Search :
 public Landmark
{
public:
    Search(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Search();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#directory
// [Deprecated in ARIA 1.2] A list of references to members of a group, such as a static table of contents.
class Directory :
 public List
{
public:
    Directory(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Directory();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#feed
// A scrollable list of articles where scrolling may cause articles to be added to or removed from either end of the list.
class Feed :
 public List
{
public:
    Feed(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Feed();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#option
// A selectable item in a listbox.
class Option :
 public Input
{
public:
    Option(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Option();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#treeitem
// An option item of a tree. This is an element within a tree that may be expanded or collapsed if it contains a sub-level group of tree item elements.
class TreeItem :
 public ListItem, public Option
{
public:
    TreeItem(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    TreeItem();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#timer
// A type of live region containing a numerical counter which indicates an amount of elapsed time from a start point, or the time remaining until an end point.
class Timer :
 public Status
{
public:
    Timer(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Timer();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#grid
// A composite widget containing a collection of one or more rows with one or more cells where some or all cells in the grid are focusable by using methods of two-dimensional navigation, such as directional arrow keys.
class Grid :
 public Composite, public Table
{
public:
    Grid(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Grid();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#heading
// A heading for a section of the page.
class Heading :
 public SectionHead
{
public:
    Heading(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Heading();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#tab
// A grouping label providing a mechanism for selecting the tab content that is to be rendered to the user.
class Tab :
 public SectionHead, public Widget
{
public:
    Tab(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Tab();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#command
// A form of widget that performs an action but does not receive input data.
class Command :
 public Widget
{
public:
    Command(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Command();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#button
// An input that allows for user-triggered actions when clicked or pressed. See related link.
class Button :
 public Command
{
public:
    Button(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Button();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#link
// An interactive reference to an internal or external resource that, when activated, causes the user agent to navigate to that resource. See related button.
class Link :
 public Command
{
public:
    Link(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Link();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#menuitem
// An option in a set of choices contained by a menu or menubar.
class MenuItem :
 public Command
{
public:
    MenuItem(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    MenuItem();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#menuitemcheckbox
// A menuitem with a checkable state whose possible values are true, false, or mixed.
class MenuItemCheckBox :
 public MenuItem
{
public:
    MenuItemCheckBox(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    MenuItemCheckBox();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#menuitemradio
// A checkable menuitem in a set of elements with the same role, only one of which can be checked at a time.
class MenuItemRadio :
 public MenuItemCheckBox
{
public:
    MenuItemRadio(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    MenuItemRadio();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#checkbox
// A checkable input that has three possible values: true, false, or mixed.
class CheckBox :
 public Input
{
public:
    CheckBox(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    CheckBox();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#combobox
// An input that controls another element, such as a listbox or grid, that can dynamically pop up to help the user set the value of the input.
class ComboBox :
 public Input
{
public:
    ComboBox(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    ComboBox();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#radio
// A checkable input in a group of elements with the same role, only one of which can be checked at a time.
class Radio :
 public Input
{
public:
    Radio(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Radio();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#textbox
// A type of input that allows free-form text as its value.
class TextBox :
 public Input
{
public:
    TextBox(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    TextBox();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#switch
// A type of checkbox that represents on/off values, as opposed to checked/unchecked values. See related checkbox.
class Switch :
 public CheckBox
{
public:
    Switch(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    Switch();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#searchbox
// A type of textbox intended for specifying search criteria. See related textbox and search.
class SearchBox :
 public TextBox
{
public:
    SearchBox(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    SearchBox();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://www.w3.org/TR/wai-aria-1.2/#tablist
// A list of tab elements, which are references to tabpanel elements.
class TabList :
 public Composite
{
public:
    TabList(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    TabList();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://w3c.github.io/graphics-aria/#graphics-document
// A type of document in which the visual appearance or layout of content conveys meaning.
class GraphicsDocument :
 public Document
{
public:
    GraphicsDocument(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    GraphicsDocument();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://w3c.github.io/graphics-aria/#graphics-object
// A section of a graphics-document that represents a distinct object or sub-component with semantic meaning.
class GraphicsObject :
 public Group
{
public:
    GraphicsObject(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    GraphicsObject();

public:
    virtual NameFromSource name_from_source() const override;
};

// https://w3c.github.io/graphics-aria/#graphics-symbol
// A graphical object used to convey a simple meaning or category, where the meaning is more important than the particular visual appearance.
class GraphicsSymbol :
 public Img
{
public:
    GraphicsSymbol(AriaData const&);

    virtual HashTable<StateAndProperties> const& supported_states() const override;
    virtual HashTable<StateAndProperties> const& supported_properties() const override;

    virtual HashTable<StateAndProperties> const& required_states() const override;
    virtual HashTable<StateAndProperties> const& required_properties() const override;

    virtual HashTable<StateAndProperties> const& prohibited_properties() const override;
    virtual HashTable<StateAndProperties> const& prohibited_states() const override;

    virtual HashTable<Role> const& required_context_roles() const override;
    virtual HashTable<Role> const& required_owned_elements() const override;
    virtual bool accessible_name_required() const override;
    virtual bool children_are_presentational() const override;
    virtual DefaultValueType default_value_for_property_or_state(StateAndProperties) const override;
protected:
    GraphicsSymbol();

public:
    virtual NameFromSource name_from_source() const override;
};
}
