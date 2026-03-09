
#include <LibWeb/ARIA/AriaRoles.h>

namespace Web::ARIA {

HashTable<StateAndProperties> const& Structure::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Structure::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Structure::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Structure::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Structure::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Structure::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Structure::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Structure::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Structure::Structure() { }

Structure::Structure(AriaData const& data)
    : RoleType(data)
{
}

bool Structure::accessible_name_required() const
{
    return false;
}

bool Structure::children_are_presentational() const
{
    return false;
}

DefaultValueType Structure::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

HashTable<StateAndProperties> const& Widget::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Widget::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Widget::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Widget::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Widget::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Widget::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Widget::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Widget::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Widget::Widget() { }

Widget::Widget(AriaData const& data)
    : RoleType(data)
{
}

bool Widget::accessible_name_required() const
{
    return false;
}

bool Widget::children_are_presentational() const
{
    return false;
}

DefaultValueType Widget::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

HashTable<StateAndProperties> const& Window::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Window::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(18);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaModal);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Window::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Window::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Window::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Window::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Window::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Window::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Window::Window() { }

Window::Window(AriaData const& data)
    : RoleType(data)
{
}

bool Window::accessible_name_required() const
{
    return false;
}

bool Window::children_are_presentational() const
{
    return false;
}

DefaultValueType Window::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Window::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Composite::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Composite::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(17);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Composite::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Composite::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Composite::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Composite::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Composite::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Composite::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Composite::Composite() { }

Composite::Composite(AriaData const& data)
    : Widget(data)
{
}

bool Composite::accessible_name_required() const
{
    return false;
}

bool Composite::children_are_presentational() const
{
    return false;
}

DefaultValueType Composite::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Composite::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Application::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(7);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaInvalid);

    }
    return states;
}

HashTable<StateAndProperties> const& Application::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(19);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Application::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Application::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Application::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Application::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Application::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Application::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Application::Application() { }

Application::Application(AriaData const& data)
    : Structure(data)
{
}

bool Application::accessible_name_required() const
{
    return true;
}

bool Application::children_are_presentational() const
{
    return false;
}

DefaultValueType Application::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Application::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Document::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Document::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Document::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Document::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Document::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Document::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Document::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Document::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Document::Document() { }

Document::Document(AriaData const& data)
    : Structure(data)
{
}

bool Document::accessible_name_required() const
{
    return false;
}

bool Document::children_are_presentational() const
{
    return false;
}

DefaultValueType Document::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Document::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Generic::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Generic::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(11);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);

    }
    return properties;
}

HashTable<StateAndProperties> const& Generic::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Generic::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Generic::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Generic::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(5);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<Role> const& Generic::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Generic::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Generic::Generic() { }

Generic::Generic(AriaData const& data)
    : Structure(data)
{
}

bool Generic::accessible_name_required() const
{
    return false;
}

bool Generic::children_are_presentational() const
{
    return false;
}

DefaultValueType Generic::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Generic::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Presentation::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Presentation::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(14);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Presentation::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Presentation::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Presentation::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Presentation::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Presentation::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Presentation::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Presentation::Presentation() { }

Presentation::Presentation(AriaData const& data)
    : Structure(data)
{
}

bool Presentation::accessible_name_required() const
{
    return false;
}

bool Presentation::children_are_presentational() const
{
    return false;
}

DefaultValueType Presentation::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Presentation::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Range::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Range::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(20);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaValueMax);
        properties.set(StateAndProperties::AriaValueMin);
        properties.set(StateAndProperties::AriaValueNow);
        properties.set(StateAndProperties::AriaValueText);

    }
    return properties;
}

HashTable<StateAndProperties> const& Range::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Range::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Range::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Range::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Range::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Range::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Range::Range() { }

Range::Range(AriaData const& data)
    : Structure(data)
{
}

bool Range::accessible_name_required() const
{
    return false;
}

bool Range::children_are_presentational() const
{
    return false;
}

DefaultValueType Range::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

HashTable<StateAndProperties> const& RowGroup::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& RowGroup::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& RowGroup::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& RowGroup::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& RowGroup::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& RowGroup::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& RowGroup::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(3);
        roles.set(Role::grid);
        roles.set(Role::table);
        roles.set(Role::treegrid);

    }
    return roles;
}

HashTable<Role> const& RowGroup::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(1);
        roles.set(Role::row);

    }
    return roles;
}

RowGroup::RowGroup() { }

RowGroup::RowGroup(AriaData const& data)
    : Structure(data)
{
}

bool RowGroup::accessible_name_required() const
{
    return false;
}

bool RowGroup::children_are_presentational() const
{
    return false;
}

DefaultValueType RowGroup::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource RowGroup::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Section::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Section::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Section::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Section::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Section::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Section::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Section::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Section::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Section::Section() { }

Section::Section(AriaData const& data)
    : Structure(data)
{
}

bool Section::accessible_name_required() const
{
    return false;
}

bool Section::children_are_presentational() const
{
    return false;
}

DefaultValueType Section::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

HashTable<StateAndProperties> const& SectionFooter::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& SectionFooter::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(17);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& SectionFooter::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& SectionFooter::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& SectionFooter::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& SectionFooter::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& SectionFooter::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& SectionFooter::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

SectionFooter::SectionFooter() { }

SectionFooter::SectionFooter(AriaData const& data)
    : Section(data)
{
}

bool SectionFooter::accessible_name_required() const
{
    return false;
}

bool SectionFooter::children_are_presentational() const
{
    return false;
}

DefaultValueType SectionFooter::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource SectionFooter::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& SectionHeader::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& SectionHeader::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(17);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& SectionHeader::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& SectionHeader::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& SectionHeader::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& SectionHeader::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& SectionHeader::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& SectionHeader::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

SectionHeader::SectionHeader() { }

SectionHeader::SectionHeader(AriaData const& data)
    : Section(data)
{
}

bool SectionHeader::accessible_name_required() const
{
    return false;
}

bool SectionHeader::children_are_presentational() const
{
    return false;
}

DefaultValueType SectionHeader::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource SectionHeader::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Input::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Input::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Input::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Input::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Input::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Input::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Input::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Input::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Input::Input() { }

Input::Input(AriaData const& data)
    : Widget(data)
{
}

bool Input::accessible_name_required() const
{
    return false;
}

bool Input::children_are_presentational() const
{
    return false;
}

DefaultValueType Input::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Input::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& SectionHead::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& SectionHead::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& SectionHead::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& SectionHead::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& SectionHead::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& SectionHead::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& SectionHead::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& SectionHead::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

SectionHead::SectionHead() { }

SectionHead::SectionHead(AriaData const& data)
    : Structure(data)
{
}

bool SectionHead::accessible_name_required() const
{
    return false;
}

bool SectionHead::children_are_presentational() const
{
    return false;
}

DefaultValueType SectionHead::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

HashTable<StateAndProperties> const& FocusableSeparator::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& FocusableSeparator::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(20);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOrientation);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaValueMax);
        properties.set(StateAndProperties::AriaValueMin);
        properties.set(StateAndProperties::AriaValueText);

    }
    return properties;
}

HashTable<StateAndProperties> const& FocusableSeparator::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& FocusableSeparator::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(1);
        properties.set(StateAndProperties::AriaValueNow);

    }
    return properties;
}

HashTable<StateAndProperties> const& FocusableSeparator::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& FocusableSeparator::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& FocusableSeparator::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& FocusableSeparator::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

FocusableSeparator::FocusableSeparator() { }

FocusableSeparator::FocusableSeparator(AriaData const& data)
    : Widget(data)
{
}

bool FocusableSeparator::accessible_name_required() const
{
    return false;
}

bool FocusableSeparator::children_are_presentational() const
{
    return true;
}

DefaultValueType FocusableSeparator::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaOrientation:
        return AriaOrientation::Horizontal;

    case StateAndProperties::AriaValueMin:
        return 0.0;

    case StateAndProperties::AriaValueMax:
        return 100.0;

    default:
        return {};
    }
}

NameFromSource FocusableSeparator::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& NonFocusableSeparator::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& NonFocusableSeparator::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(17);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOrientation);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& NonFocusableSeparator::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& NonFocusableSeparator::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& NonFocusableSeparator::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& NonFocusableSeparator::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& NonFocusableSeparator::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& NonFocusableSeparator::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

NonFocusableSeparator::NonFocusableSeparator() { }

NonFocusableSeparator::NonFocusableSeparator(AriaData const& data)
    : Structure(data)
{
}

bool NonFocusableSeparator::accessible_name_required() const
{
    return false;
}

bool NonFocusableSeparator::children_are_presentational() const
{
    return true;
}

DefaultValueType NonFocusableSeparator::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaOrientation:
        return AriaOrientation::Horizontal;

    default:
        return {};
    }
}

NameFromSource NonFocusableSeparator::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Article::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Article::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(18);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaPosInSet);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaSetSize);

    }
    return properties;
}

HashTable<StateAndProperties> const& Article::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Article::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Article::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Article::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Article::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Article::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Article::Article() { }

Article::Article(AriaData const& data)
    : Document(data)
{
}

bool Article::accessible_name_required() const
{
    return false;
}

bool Article::children_are_presentational() const
{
    return false;
}

DefaultValueType Article::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Article::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Meter::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Meter::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(19);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaValueMax);
        properties.set(StateAndProperties::AriaValueMin);
        properties.set(StateAndProperties::AriaValueText);

    }
    return properties;
}

HashTable<StateAndProperties> const& Meter::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Meter::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(1);
        properties.set(StateAndProperties::AriaValueNow);

    }
    return properties;
}

HashTable<StateAndProperties> const& Meter::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Meter::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Meter::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Meter::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Meter::Meter() { }

Meter::Meter(AriaData const& data)
    : Range(data)
{
}

bool Meter::accessible_name_required() const
{
    return true;
}

bool Meter::children_are_presentational() const
{
    return true;
}

DefaultValueType Meter::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaValueMin:
        return 0.0;

    case StateAndProperties::AriaValueMax:
        return 100.0;

    default:
        return {};
    }
}

NameFromSource Meter::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Progressbar::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Progressbar::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(20);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaValueMax);
        properties.set(StateAndProperties::AriaValueMin);
        properties.set(StateAndProperties::AriaValueNow);
        properties.set(StateAndProperties::AriaValueText);

    }
    return properties;
}

HashTable<StateAndProperties> const& Progressbar::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Progressbar::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Progressbar::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Progressbar::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Progressbar::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Progressbar::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Progressbar::Progressbar() { }

Progressbar::Progressbar(AriaData const& data)
    : Range(data)
{
}

bool Progressbar::accessible_name_required() const
{
    return true;
}

bool Progressbar::children_are_presentational() const
{
    return true;
}

DefaultValueType Progressbar::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaValueMin:
        return 0.0;

    case StateAndProperties::AriaValueMax:
        return 100.0;

    default:
        return {};
    }
}

NameFromSource Progressbar::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Scrollbar::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Scrollbar::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(20);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOrientation);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaValueMax);
        properties.set(StateAndProperties::AriaValueMin);
        properties.set(StateAndProperties::AriaValueText);

    }
    return properties;
}

HashTable<StateAndProperties> const& Scrollbar::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Scrollbar::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(2);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaValueNow);

    }
    return properties;
}

HashTable<StateAndProperties> const& Scrollbar::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Scrollbar::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Scrollbar::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Scrollbar::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Scrollbar::Scrollbar() { }

Scrollbar::Scrollbar(AriaData const& data)
    : Range(data)
{
}

bool Scrollbar::accessible_name_required() const
{
    return false;
}

bool Scrollbar::children_are_presentational() const
{
    return true;
}

DefaultValueType Scrollbar::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaOrientation:
        return AriaOrientation::Vertical;

    case StateAndProperties::AriaValueMin:
        return 0.0;

    case StateAndProperties::AriaValueMax:
        return 100.0;

    default:
        return {};
    }
}

NameFromSource Scrollbar::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Slider::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Slider::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(22);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOrientation);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaReadOnly);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaValueMax);
        properties.set(StateAndProperties::AriaValueMin);

    }
    return properties;
}

HashTable<StateAndProperties> const& Slider::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Slider::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(1);
        properties.set(StateAndProperties::AriaValueNow);

    }
    return properties;
}

HashTable<StateAndProperties> const& Slider::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Slider::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Slider::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Slider::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Slider::Slider() { }

Slider::Slider(AriaData const& data)
    : Input(data)
{
}

bool Slider::accessible_name_required() const
{
    return true;
}

bool Slider::children_are_presentational() const
{
    return true;
}

DefaultValueType Slider::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaOrientation:
        return AriaOrientation::Horizontal;

    case StateAndProperties::AriaValueMin:
        return 0.0;

    case StateAndProperties::AriaValueMax:
        return 100.0;

    default:
        return {};
    }
}

NameFromSource Slider::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& SpinButton::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& SpinButton::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(23);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaReadOnly);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaValueMax);
        properties.set(StateAndProperties::AriaValueMin);
        properties.set(StateAndProperties::AriaValueNow);
        properties.set(StateAndProperties::AriaValueText);

    }
    return properties;
}

HashTable<StateAndProperties> const& SpinButton::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& SpinButton::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& SpinButton::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& SpinButton::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& SpinButton::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& SpinButton::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

SpinButton::SpinButton() { }

SpinButton::SpinButton(AriaData const& data)
    : Composite(data)
{
}

bool SpinButton::accessible_name_required() const
{
    return true;
}

bool SpinButton::children_are_presentational() const
{
    return false;
}

DefaultValueType SpinButton::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaValueNow:
        return 0.0;

    default:
        return {};
    }
}

NameFromSource SpinButton::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Alert::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Alert::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Alert::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Alert::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Alert::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Alert::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Alert::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Alert::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Alert::Alert() { }

Alert::Alert(AriaData const& data)
    : Section(data)
{
}

bool Alert::accessible_name_required() const
{
    return false;
}

bool Alert::children_are_presentational() const
{
    return false;
}

DefaultValueType Alert::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaLive:
        return AriaLive::Assertive;

    case StateAndProperties::AriaAtomic:
        return true;

    default:
        return {};
    }
}

NameFromSource Alert::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& BlockQuote::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& BlockQuote::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& BlockQuote::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& BlockQuote::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& BlockQuote::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& BlockQuote::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& BlockQuote::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& BlockQuote::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

BlockQuote::BlockQuote() { }

BlockQuote::BlockQuote(AriaData const& data)
    : Section(data)
{
}

bool BlockQuote::accessible_name_required() const
{
    return false;
}

bool BlockQuote::children_are_presentational() const
{
    return false;
}

DefaultValueType BlockQuote::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource BlockQuote::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Caption::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Caption::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(13);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Caption::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Caption::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Caption::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Caption::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Caption::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(6);
        roles.set(Role::figure);
        roles.set(Role::grid);
        roles.set(Role::group);
        roles.set(Role::radiogroup);
        roles.set(Role::table);
        roles.set(Role::treegrid);

    }
    return roles;
}

HashTable<Role> const& Caption::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Caption::Caption() { }

Caption::Caption(AriaData const& data)
    : Section(data)
{
}

bool Caption::accessible_name_required() const
{
    return false;
}

bool Caption::children_are_presentational() const
{
    return false;
}

DefaultValueType Caption::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Caption::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Cell::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Cell::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(22);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaColIndex);
        properties.set(StateAndProperties::AriaColIndexText);
        properties.set(StateAndProperties::AriaColSpan);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaRowIndex);
        properties.set(StateAndProperties::AriaRowIndexText);
        properties.set(StateAndProperties::AriaRowSpan);

    }
    return properties;
}

HashTable<StateAndProperties> const& Cell::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Cell::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Cell::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Cell::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Cell::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(1);
        roles.set(Role::row);

    }
    return roles;
}

HashTable<Role> const& Cell::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Cell::Cell() { }

Cell::Cell(AriaData const& data)
    : Section(data)
{
}

bool Cell::accessible_name_required() const
{
    return false;
}

bool Cell::children_are_presentational() const
{
    return false;
}

DefaultValueType Cell::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Cell::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& Code::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Code::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(13);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Code::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Code::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Code::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Code::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Code::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Code::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Code::Code() { }

Code::Code(AriaData const& data)
    : Section(data)
{
}

bool Code::accessible_name_required() const
{
    return false;
}

bool Code::children_are_presentational() const
{
    return false;
}

DefaultValueType Code::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Code::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Definition::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Definition::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(15);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Definition::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Definition::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Definition::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Definition::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Definition::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Definition::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Definition::Definition() { }

Definition::Definition(AriaData const& data)
    : Section(data)
{
}

bool Definition::accessible_name_required() const
{
    return false;
}

bool Definition::children_are_presentational() const
{
    return false;
}

DefaultValueType Definition::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Definition::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Deletion::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Deletion::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(13);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Deletion::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Deletion::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Deletion::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Deletion::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Deletion::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Deletion::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Deletion::Deletion() { }

Deletion::Deletion(AriaData const& data)
    : Section(data)
{
}

bool Deletion::accessible_name_required() const
{
    return false;
}

bool Deletion::children_are_presentational() const
{
    return false;
}

DefaultValueType Deletion::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Deletion::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Emphasis::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Emphasis::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(13);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Emphasis::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Emphasis::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Emphasis::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Emphasis::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Emphasis::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Emphasis::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Emphasis::Emphasis() { }

Emphasis::Emphasis(AriaData const& data)
    : Section(data)
{
}

bool Emphasis::accessible_name_required() const
{
    return false;
}

bool Emphasis::children_are_presentational() const
{
    return false;
}

DefaultValueType Emphasis::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Emphasis::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Figure::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Figure::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Figure::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Figure::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Figure::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Figure::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Figure::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Figure::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Figure::Figure() { }

Figure::Figure(AriaData const& data)
    : Section(data)
{
}

bool Figure::accessible_name_required() const
{
    return false;
}

bool Figure::children_are_presentational() const
{
    return false;
}

DefaultValueType Figure::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Figure::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Group::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Group::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(17);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Group::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Group::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Group::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Group::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Group::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Group::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Group::Group() { }

Group::Group(AriaData const& data)
    : Section(data)
{
}

bool Group::accessible_name_required() const
{
    return false;
}

bool Group::children_are_presentational() const
{
    return false;
}

DefaultValueType Group::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Group::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Img::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Img::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Img::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Img::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Img::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Img::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Img::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Img::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Img::Img() { }

Img::Img(AriaData const& data)
    : Section(data)
{
}

bool Img::accessible_name_required() const
{
    return true;
}

bool Img::children_are_presentational() const
{
    return true;
}

DefaultValueType Img::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Img::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Insertion::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Insertion::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(13);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Insertion::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Insertion::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Insertion::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Insertion::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Insertion::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Insertion::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Insertion::Insertion() { }

Insertion::Insertion(AriaData const& data)
    : Section(data)
{
}

bool Insertion::accessible_name_required() const
{
    return false;
}

bool Insertion::children_are_presentational() const
{
    return false;
}

DefaultValueType Insertion::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Insertion::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Landmark::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Landmark::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Landmark::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Landmark::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Landmark::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Landmark::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Landmark::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Landmark::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Landmark::Landmark() { }

Landmark::Landmark(AriaData const& data)
    : Section(data)
{
}

bool Landmark::accessible_name_required() const
{
    return false;
}

bool Landmark::children_are_presentational() const
{
    return false;
}

DefaultValueType Landmark::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Landmark::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& List::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& List::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& List::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& List::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& List::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& List::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& List::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& List::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(1);
        roles.set(Role::listitem);

    }
    return roles;
}

List::List() { }

List::List(AriaData const& data)
    : Section(data)
{
}

bool List::accessible_name_required() const
{
    return false;
}

bool List::children_are_presentational() const
{
    return false;
}

DefaultValueType List::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource List::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& ListItem::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& ListItem::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(19);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLevel);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaPosInSet);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaSetSize);

    }
    return properties;
}

HashTable<StateAndProperties> const& ListItem::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& ListItem::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& ListItem::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& ListItem::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& ListItem::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(2);
        roles.set(Role::directory);
        roles.set(Role::list);

    }
    return roles;
}

HashTable<Role> const& ListItem::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

ListItem::ListItem() { }

ListItem::ListItem(AriaData const& data)
    : Section(data)
{
}

bool ListItem::accessible_name_required() const
{
    return false;
}

bool ListItem::children_are_presentational() const
{
    return false;
}

DefaultValueType ListItem::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource ListItem::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Log::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Log::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Log::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Log::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Log::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Log::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Log::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Log::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Log::Log() { }

Log::Log(AriaData const& data)
    : Section(data)
{
}

bool Log::accessible_name_required() const
{
    return false;
}

bool Log::children_are_presentational() const
{
    return false;
}

DefaultValueType Log::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaLive:
        return AriaLive::Polite;

    default:
        return {};
    }
}

NameFromSource Log::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Marquee::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Marquee::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Marquee::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Marquee::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Marquee::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Marquee::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Marquee::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Marquee::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Marquee::Marquee() { }

Marquee::Marquee(AriaData const& data)
    : Section(data)
{
}

bool Marquee::accessible_name_required() const
{
    return false;
}

bool Marquee::children_are_presentational() const
{
    return false;
}

DefaultValueType Marquee::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Marquee::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Math::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Math::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Math::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Math::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Math::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Math::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Math::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Math::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Math::Math() { }

Math::Math(AriaData const& data)
    : Section(data)
{
}

bool Math::accessible_name_required() const
{
    return false;
}

bool Math::children_are_presentational() const
{
    return false;
}

DefaultValueType Math::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Math::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Note::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Note::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Note::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Note::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Note::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Note::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Note::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Note::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Note::Note() { }

Note::Note(AriaData const& data)
    : Section(data)
{
}

bool Note::accessible_name_required() const
{
    return false;
}

bool Note::children_are_presentational() const
{
    return false;
}

DefaultValueType Note::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Note::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Paragraph::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Paragraph::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(13);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Paragraph::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Paragraph::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Paragraph::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Paragraph::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Paragraph::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Paragraph::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Paragraph::Paragraph() { }

Paragraph::Paragraph(AriaData const& data)
    : Section(data)
{
}

bool Paragraph::accessible_name_required() const
{
    return false;
}

bool Paragraph::children_are_presentational() const
{
    return false;
}

DefaultValueType Paragraph::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Paragraph::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Status::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Status::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Status::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Status::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Status::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Status::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Status::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Status::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Status::Status() { }

Status::Status(AriaData const& data)
    : Section(data)
{
}

bool Status::accessible_name_required() const
{
    return false;
}

bool Status::children_are_presentational() const
{
    return false;
}

DefaultValueType Status::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaLive:
        return AriaLive::Polite;

    case StateAndProperties::AriaAtomic:
        return true;

    default:
        return {};
    }
}

NameFromSource Status::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Strong::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Strong::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(13);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Strong::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Strong::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Strong::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Strong::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Strong::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Strong::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Strong::Strong() { }

Strong::Strong(AriaData const& data)
    : Section(data)
{
}

bool Strong::accessible_name_required() const
{
    return false;
}

bool Strong::children_are_presentational() const
{
    return false;
}

DefaultValueType Strong::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Strong::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Subscript::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Subscript::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(13);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Subscript::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Subscript::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Subscript::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Subscript::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Subscript::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Subscript::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Subscript::Subscript() { }

Subscript::Subscript(AriaData const& data)
    : Section(data)
{
}

bool Subscript::accessible_name_required() const
{
    return false;
}

bool Subscript::children_are_presentational() const
{
    return false;
}

DefaultValueType Subscript::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Subscript::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Suggestion::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Suggestion::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(13);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Suggestion::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Suggestion::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Suggestion::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Suggestion::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Suggestion::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Suggestion::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(2);
        roles.set(Role::insertion);
        roles.set(Role::deletion);

    }
    return roles;
}

Suggestion::Suggestion() { }

Suggestion::Suggestion(AriaData const& data)
    : Section(data)
{
}

bool Suggestion::accessible_name_required() const
{
    return false;
}

bool Suggestion::children_are_presentational() const
{
    return false;
}

DefaultValueType Suggestion::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Suggestion::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Superscript::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Superscript::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(13);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Superscript::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Superscript::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Superscript::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Superscript::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Superscript::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Superscript::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Superscript::Superscript() { }

Superscript::Superscript(AriaData const& data)
    : Section(data)
{
}

bool Superscript::accessible_name_required() const
{
    return false;
}

bool Superscript::children_are_presentational() const
{
    return false;
}

DefaultValueType Superscript::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Superscript::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Table::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Table::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(18);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaColCount);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaRowCount);

    }
    return properties;
}

HashTable<StateAndProperties> const& Table::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Table::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Table::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Table::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Table::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Table::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(3);
        roles.set(Role::caption);
        roles.set(Role::row);
        roles.set(Role::rowgroup);

    }
    return roles;
}

Table::Table() { }

Table::Table(AriaData const& data)
    : Section(data)
{
}

bool Table::accessible_name_required() const
{
    return true;
}

bool Table::children_are_presentational() const
{
    return false;
}

DefaultValueType Table::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Table::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& TabPanel::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& TabPanel::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& TabPanel::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& TabPanel::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& TabPanel::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& TabPanel::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& TabPanel::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& TabPanel::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

TabPanel::TabPanel() { }

TabPanel::TabPanel(AriaData const& data)
    : Section(data)
{
}

bool TabPanel::accessible_name_required() const
{
    return true;
}

bool TabPanel::children_are_presentational() const
{
    return false;
}

DefaultValueType TabPanel::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource TabPanel::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Term::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Term::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(15);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Term::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Term::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Term::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Term::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Term::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Term::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Term::Term() { }

Term::Term(AriaData const& data)
    : Section(data)
{
}

bool Term::accessible_name_required() const
{
    return false;
}

bool Term::children_are_presentational() const
{
    return false;
}

DefaultValueType Term::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Term::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Time::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Time::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(15);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Time::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Time::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Time::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Time::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Time::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Time::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Time::Time() { }

Time::Time(AriaData const& data)
    : Section(data)
{
}

bool Time::accessible_name_required() const
{
    return false;
}

bool Time::children_are_presentational() const
{
    return false;
}

DefaultValueType Time::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Time::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Tooltip::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Tooltip::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Tooltip::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Tooltip::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Tooltip::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Tooltip::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Tooltip::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Tooltip::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Tooltip::Tooltip() { }

Tooltip::Tooltip(AriaData const& data)
    : Section(data)
{
}

bool Tooltip::accessible_name_required() const
{
    return false;
}

bool Tooltip::children_are_presentational() const
{
    return false;
}

DefaultValueType Tooltip::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Tooltip::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& Dialog::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Dialog::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(17);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaModal);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Dialog::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Dialog::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Dialog::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Dialog::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Dialog::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Dialog::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Dialog::Dialog() { }

Dialog::Dialog(AriaData const& data)
    : Window(data)
{
}

bool Dialog::accessible_name_required() const
{
    return true;
}

bool Dialog::children_are_presentational() const
{
    return false;
}

DefaultValueType Dialog::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Dialog::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& AlertDialog::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& AlertDialog::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(17);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaModal);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& AlertDialog::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& AlertDialog::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& AlertDialog::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& AlertDialog::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& AlertDialog::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& AlertDialog::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

AlertDialog::AlertDialog() { }

AlertDialog::AlertDialog(AriaData const& data)
    : Alert(data)
{
}

bool AlertDialog::accessible_name_required() const
{
    return true;
}

bool AlertDialog::children_are_presentational() const
{
    return false;
}

DefaultValueType AlertDialog::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource AlertDialog::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& GridCell::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(8);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaInvalid);
        states.set(StateAndProperties::AriaSelected);

    }
    return states;
}

HashTable<StateAndProperties> const& GridCell::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(26);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaColIndex);
        properties.set(StateAndProperties::AriaColIndexText);
        properties.set(StateAndProperties::AriaColSpan);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaReadOnly);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaRowIndex);
        properties.set(StateAndProperties::AriaRowIndexText);
        properties.set(StateAndProperties::AriaRowSpan);

    }
    return properties;
}

HashTable<StateAndProperties> const& GridCell::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& GridCell::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& GridCell::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& GridCell::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& GridCell::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(1);
        roles.set(Role::row);

    }
    return roles;
}

HashTable<Role> const& GridCell::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

GridCell::GridCell() { }

GridCell::GridCell(AriaData const& data)
    : Cell(data)
{
}

bool GridCell::accessible_name_required() const
{
    return false;
}

bool GridCell::children_are_presentational() const
{
    return false;
}

DefaultValueType GridCell::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource GridCell::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& ColumnHeader::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(8);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaInvalid);
        states.set(StateAndProperties::AriaSelected);

    }
    return states;
}

HashTable<StateAndProperties> const& ColumnHeader::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(27);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaColIndex);
        properties.set(StateAndProperties::AriaColIndexText);
        properties.set(StateAndProperties::AriaColSpan);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaReadOnly);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaRowIndex);
        properties.set(StateAndProperties::AriaRowIndexText);
        properties.set(StateAndProperties::AriaRowSpan);
        properties.set(StateAndProperties::AriaSort);

    }
    return properties;
}

HashTable<StateAndProperties> const& ColumnHeader::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& ColumnHeader::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& ColumnHeader::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& ColumnHeader::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& ColumnHeader::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(1);
        roles.set(Role::row);

    }
    return roles;
}

HashTable<Role> const& ColumnHeader::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

ColumnHeader::ColumnHeader() { }

ColumnHeader::ColumnHeader(AriaData const& data)
    : GridCell(data)
{
}

bool ColumnHeader::accessible_name_required() const
{
    return true;
}

bool ColumnHeader::children_are_presentational() const
{
    return false;
}

DefaultValueType ColumnHeader::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource ColumnHeader::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& RowHeader::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(8);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaInvalid);
        states.set(StateAndProperties::AriaSelected);

    }
    return states;
}

HashTable<StateAndProperties> const& RowHeader::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(23);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaColIndex);
        properties.set(StateAndProperties::AriaColSpan);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaRowIndex);
        properties.set(StateAndProperties::AriaRowSpan);
        properties.set(StateAndProperties::AriaSort);

    }
    return properties;
}

HashTable<StateAndProperties> const& RowHeader::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& RowHeader::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& RowHeader::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& RowHeader::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& RowHeader::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(1);
        roles.set(Role::row);

    }
    return roles;
}

HashTable<Role> const& RowHeader::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

RowHeader::RowHeader() { }

RowHeader::RowHeader(AriaData const& data)
    : GridCell(data)
{
}

bool RowHeader::accessible_name_required() const
{
    return true;
}

bool RowHeader::children_are_presentational() const
{
    return false;
}

DefaultValueType RowHeader::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource RowHeader::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& Row::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(7);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaSelected);

    }
    return states;
}

HashTable<StateAndProperties> const& Row::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(24);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaColIndex);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaExpanded);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLevel);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaPosInSet);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaRowIndex);
        properties.set(StateAndProperties::AriaRowIndexText);
        properties.set(StateAndProperties::AriaSetSize);

    }
    return properties;
}

HashTable<StateAndProperties> const& Row::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Row::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Row::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Row::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Row::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(4);
        roles.set(Role::grid);
        roles.set(Role::rowgroup);
        roles.set(Role::table);
        roles.set(Role::treegrid);

    }
    return roles;
}

HashTable<Role> const& Row::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(4);
        roles.set(Role::cell);
        roles.set(Role::columnheader);
        roles.set(Role::gridcell);
        roles.set(Role::rowheader);

    }
    return roles;
}

Row::Row() { }

Row::Row(AriaData const& data)
    : Group(data)
{
}

bool Row::accessible_name_required() const
{
    return false;
}

bool Row::children_are_presentational() const
{
    return false;
}

DefaultValueType Row::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Row::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& Select::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Select::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(18);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOrientation);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Select::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Select::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Select::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Select::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Select::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Select::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Select::Select() { }

Select::Select(AriaData const& data)
    : Composite(data)
{
}

bool Select::accessible_name_required() const
{
    return false;
}

bool Select::children_are_presentational() const
{
    return false;
}

DefaultValueType Select::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Select::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Toolbar::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Toolbar::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(18);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOrientation);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Toolbar::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Toolbar::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Toolbar::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Toolbar::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Toolbar::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Toolbar::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Toolbar::Toolbar() { }

Toolbar::Toolbar(AriaData const& data)
    : Group(data)
{
}

bool Toolbar::accessible_name_required() const
{
    return false;
}

bool Toolbar::children_are_presentational() const
{
    return false;
}

DefaultValueType Toolbar::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaOrientation:
        return AriaOrientation::Horizontal;

    default:
        return {};
    }
}

NameFromSource Toolbar::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& ListBox::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(7);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaInvalid);

    }
    return states;
}

HashTable<StateAndProperties> const& ListBox::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(23);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaExpanded);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaMultiSelectable);
        properties.set(StateAndProperties::AriaOrientation);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaReadOnly);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& ListBox::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& ListBox::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& ListBox::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& ListBox::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& ListBox::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& ListBox::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(2);
        roles.set(Role::group);
        roles.set(Role::option);

    }
    return roles;
}

ListBox::ListBox() { }

ListBox::ListBox(AriaData const& data)
    : Select(data)
{
}

bool ListBox::accessible_name_required() const
{
    return true;
}

bool ListBox::children_are_presentational() const
{
    return false;
}

DefaultValueType ListBox::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaOrientation:
        return AriaOrientation::Vertical;

    default:
        return {};
    }
}

NameFromSource ListBox::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Menu::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Menu::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(18);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOrientation);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Menu::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Menu::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Menu::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Menu::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Menu::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Menu::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(5);
        roles.set(Role::group);
        roles.set(Role::menuitem);
        roles.set(Role::menuitemradio);
        roles.set(Role::menuitemcheckbox);
        roles.set(Role::separator);

    }
    return roles;
}

Menu::Menu() { }

Menu::Menu(AriaData const& data)
    : Select(data)
{
}

bool Menu::accessible_name_required() const
{
    return false;
}

bool Menu::children_are_presentational() const
{
    return false;
}

DefaultValueType Menu::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaOrientation:
        return AriaOrientation::Vertical;

    default:
        return {};
    }
}

NameFromSource Menu::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& RadioGroup::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(6);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaInvalid);

    }
    return states;
}

HashTable<StateAndProperties> const& RadioGroup::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(21);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOrientation);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaReadOnly);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& RadioGroup::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& RadioGroup::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& RadioGroup::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& RadioGroup::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& RadioGroup::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& RadioGroup::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

RadioGroup::RadioGroup() { }

RadioGroup::RadioGroup(AriaData const& data)
    : Select(data)
{
}

bool RadioGroup::accessible_name_required() const
{
    return true;
}

bool RadioGroup::children_are_presentational() const
{
    return false;
}

DefaultValueType RadioGroup::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource RadioGroup::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Tree::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(6);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaInvalid);

    }
    return states;
}

HashTable<StateAndProperties> const& Tree::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(21);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaMultiSelectable);
        properties.set(StateAndProperties::AriaOrientation);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Tree::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Tree::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Tree::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Tree::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Tree::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Tree::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(2);
        roles.set(Role::group);
        roles.set(Role::treeitem);

    }
    return roles;
}

Tree::Tree() { }

Tree::Tree(AriaData const& data)
    : Select(data)
{
}

bool Tree::accessible_name_required() const
{
    return true;
}

bool Tree::children_are_presentational() const
{
    return false;
}

DefaultValueType Tree::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaOrientation:
        return AriaOrientation::Vertical;

    default:
        return {};
    }
}

NameFromSource Tree::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& MenuBar::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& MenuBar::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(17);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& MenuBar::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& MenuBar::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& MenuBar::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& MenuBar::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& MenuBar::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& MenuBar::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(5);
        roles.set(Role::group);
        roles.set(Role::menuitem);
        roles.set(Role::menuitemradio);
        roles.set(Role::menuitemcheckbox);
        roles.set(Role::separator);

    }
    return roles;
}

MenuBar::MenuBar() { }

MenuBar::MenuBar(AriaData const& data)
    : Menu(data)
{
}

bool MenuBar::accessible_name_required() const
{
    return false;
}

bool MenuBar::children_are_presentational() const
{
    return false;
}

DefaultValueType MenuBar::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaOrientation:
        return AriaOrientation::Horizontal;

    default:
        return {};
    }
}

NameFromSource MenuBar::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& TreeGrid::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(6);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaInvalid);

    }
    return states;
}

HashTable<StateAndProperties> const& TreeGrid::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(23);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaColCount);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaMultiSelectable);
        properties.set(StateAndProperties::AriaOrientation);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaReadOnly);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaRowCount);

    }
    return properties;
}

HashTable<StateAndProperties> const& TreeGrid::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& TreeGrid::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& TreeGrid::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& TreeGrid::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& TreeGrid::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& TreeGrid::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(3);
        roles.set(Role::caption);
        roles.set(Role::row);
        roles.set(Role::rowgroup);

    }
    return roles;
}

TreeGrid::TreeGrid() { }

TreeGrid::TreeGrid(AriaData const& data)
    : Row(data)
{
}

bool TreeGrid::accessible_name_required() const
{
    return true;
}

bool TreeGrid::children_are_presentational() const
{
    return false;
}

DefaultValueType TreeGrid::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource TreeGrid::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Banner::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Banner::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Banner::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Banner::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Banner::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Banner::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Banner::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Banner::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Banner::Banner() { }

Banner::Banner(AriaData const& data)
    : Landmark(data)
{
}

bool Banner::accessible_name_required() const
{
    return false;
}

bool Banner::children_are_presentational() const
{
    return false;
}

DefaultValueType Banner::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Banner::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Complementary::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Complementary::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Complementary::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Complementary::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Complementary::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Complementary::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Complementary::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Complementary::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Complementary::Complementary() { }

Complementary::Complementary(AriaData const& data)
    : Landmark(data)
{
}

bool Complementary::accessible_name_required() const
{
    return false;
}

bool Complementary::children_are_presentational() const
{
    return false;
}

DefaultValueType Complementary::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Complementary::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& ContentInfo::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& ContentInfo::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& ContentInfo::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& ContentInfo::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& ContentInfo::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& ContentInfo::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& ContentInfo::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& ContentInfo::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

ContentInfo::ContentInfo() { }

ContentInfo::ContentInfo(AriaData const& data)
    : Landmark(data)
{
}

bool ContentInfo::accessible_name_required() const
{
    return false;
}

bool ContentInfo::children_are_presentational() const
{
    return false;
}

DefaultValueType ContentInfo::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource ContentInfo::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Form::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Form::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Form::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Form::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Form::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Form::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Form::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Form::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Form::Form() { }

Form::Form(AriaData const& data)
    : Landmark(data)
{
}

bool Form::accessible_name_required() const
{
    return true;
}

bool Form::children_are_presentational() const
{
    return false;
}

DefaultValueType Form::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Form::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Main::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Main::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Main::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Main::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Main::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Main::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Main::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Main::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Main::Main() { }

Main::Main(AriaData const& data)
    : Landmark(data)
{
}

bool Main::accessible_name_required() const
{
    return false;
}

bool Main::children_are_presentational() const
{
    return false;
}

DefaultValueType Main::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Main::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Mark::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Mark::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(13);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Mark::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Mark::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Mark::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Mark::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(3);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);

    }
    return properties;
}

HashTable<Role> const& Mark::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Mark::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Mark::Mark() { }

Mark::Mark(AriaData const& data)
    : Section(data)
{
}

bool Mark::accessible_name_required() const
{
    return false;
}

bool Mark::children_are_presentational() const
{
    return false;
}

DefaultValueType Mark::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Mark::name_from_source() const
{
    return NameFromSource::Prohibited;
}

HashTable<StateAndProperties> const& Navigation::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Navigation::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Navigation::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Navigation::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Navigation::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Navigation::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Navigation::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Navigation::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Navigation::Navigation() { }

Navigation::Navigation(AriaData const& data)
    : Landmark(data)
{
}

bool Navigation::accessible_name_required() const
{
    return false;
}

bool Navigation::children_are_presentational() const
{
    return false;
}

DefaultValueType Navigation::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Navigation::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Region::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Region::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Region::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Region::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Region::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Region::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Region::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Region::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Region::Region() { }

Region::Region(AriaData const& data)
    : Landmark(data)
{
}

bool Region::accessible_name_required() const
{
    return true;
}

bool Region::children_are_presentational() const
{
    return false;
}

DefaultValueType Region::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Region::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Search::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Search::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Search::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Search::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Search::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Search::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Search::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Search::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Search::Search() { }

Search::Search(AriaData const& data)
    : Landmark(data)
{
}

bool Search::accessible_name_required() const
{
    return false;
}

bool Search::children_are_presentational() const
{
    return false;
}

DefaultValueType Search::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Search::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Directory::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Directory::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Directory::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Directory::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Directory::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Directory::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Directory::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Directory::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Directory::Directory() { }

Directory::Directory(AriaData const& data)
    : List(data)
{
}

bool Directory::accessible_name_required() const
{
    return false;
}

bool Directory::children_are_presentational() const
{
    return false;
}

DefaultValueType Directory::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Directory::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Feed::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Feed::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Feed::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Feed::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Feed::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Feed::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Feed::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Feed::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(1);
        roles.set(Role::article);

    }
    return roles;
}

Feed::Feed() { }

Feed::Feed(AriaData const& data)
    : List(data)
{
}

bool Feed::accessible_name_required() const
{
    return false;
}

bool Feed::children_are_presentational() const
{
    return false;
}

DefaultValueType Feed::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Feed::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Option::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(6);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaChecked);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Option::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(18);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaPosInSet);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaSetSize);

    }
    return properties;
}

HashTable<StateAndProperties> const& Option::required_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(1);
        states.set(StateAndProperties::AriaSelected);

    }
    return states;
}

HashTable<StateAndProperties> const& Option::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Option::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Option::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Option::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(2);
        roles.set(Role::group);
        roles.set(Role::listbox);

    }
    return roles;
}

HashTable<Role> const& Option::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Option::Option() { }

Option::Option(AriaData const& data)
    : Input(data)
{
}

bool Option::accessible_name_required() const
{
    return true;
}

bool Option::children_are_presentational() const
{
    return true;
}

DefaultValueType Option::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Option::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& TreeItem::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(8);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaChecked);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaSelected);

    }
    return states;
}

HashTable<StateAndProperties> const& TreeItem::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(19);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLevel);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaSetSize);

    }
    return properties;
}

HashTable<StateAndProperties> const& TreeItem::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& TreeItem::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& TreeItem::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& TreeItem::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& TreeItem::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(2);
        roles.set(Role::group);
        roles.set(Role::tree);

    }
    return roles;
}

HashTable<Role> const& TreeItem::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

TreeItem::TreeItem() { }

TreeItem::TreeItem(AriaData const& data)
    : ListItem(data)
{
}

bool TreeItem::accessible_name_required() const
{
    return true;
}

bool TreeItem::children_are_presentational() const
{
    return false;
}

DefaultValueType TreeItem::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource TreeItem::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& Timer::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Timer::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Timer::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Timer::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Timer::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Timer::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Timer::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Timer::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Timer::Timer() { }

Timer::Timer(AriaData const& data)
    : Status(data)
{
}

bool Timer::accessible_name_required() const
{
    return false;
}

bool Timer::children_are_presentational() const
{
    return false;
}

DefaultValueType Timer::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaLive:
        return AriaLive::Off;

    default:
        return {};
    }
}

NameFromSource Timer::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Grid::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Grid::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(20);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaColCount);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaMultiSelectable);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaReadOnly);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Grid::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Grid::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Grid::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Grid::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Grid::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Grid::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(3);
        roles.set(Role::caption);
        roles.set(Role::row);
        roles.set(Role::rowgroup);

    }
    return roles;
}

Grid::Grid() { }

Grid::Grid(AriaData const& data)
    : Composite(data)
{
}

bool Grid::accessible_name_required() const
{
    return true;
}

bool Grid::children_are_presentational() const
{
    return false;
}

DefaultValueType Grid::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Grid::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Heading::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Heading::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(17);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLevel);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Heading::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Heading::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(1);
        properties.set(StateAndProperties::AriaLevel);

    }
    return properties;
}

HashTable<StateAndProperties> const& Heading::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Heading::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Heading::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Heading::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Heading::Heading() { }

Heading::Heading(AriaData const& data)
    : SectionHead(data)
{
}

bool Heading::accessible_name_required() const
{
    return true;
}

bool Heading::children_are_presentational() const
{
    return false;
}

DefaultValueType Heading::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Heading::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& Tab::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(7);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaSelected);

    }
    return states;
}

HashTable<StateAndProperties> const& Tab::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(19);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaPosInSet);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaSetSize);

    }
    return properties;
}

HashTable<StateAndProperties> const& Tab::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Tab::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Tab::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Tab::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Tab::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(1);
        roles.set(Role::tablist);

    }
    return roles;
}

HashTable<Role> const& Tab::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Tab::Tab() { }

Tab::Tab(AriaData const& data)
    : SectionHead(data)
{
}

bool Tab::accessible_name_required() const
{
    return true;
}

bool Tab::children_are_presentational() const
{
    return true;
}

DefaultValueType Tab::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaSelected:
        return false;

    default:
        return {};
    }
}

NameFromSource Tab::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& Command::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Command::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Command::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Command::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Command::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Command::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Command::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Command::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Command::Command() { }

Command::Command(AriaData const& data)
    : Widget(data)
{
}

bool Command::accessible_name_required() const
{
    return false;
}

bool Command::children_are_presentational() const
{
    return false;
}

DefaultValueType Command::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Command::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Button::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(7);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaPressed);

    }
    return states;
}

HashTable<StateAndProperties> const& Button::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(17);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Button::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Button::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Button::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Button::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Button::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Button::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Button::Button() { }

Button::Button(AriaData const& data)
    : Command(data)
{
}

bool Button::accessible_name_required() const
{
    return true;
}

bool Button::children_are_presentational() const
{
    return true;
}

DefaultValueType Button::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Button::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& Link::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(6);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Link::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(17);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Link::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Link::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Link::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Link::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Link::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Link::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Link::Link() { }

Link::Link(AriaData const& data)
    : Command(data)
{
}

bool Link::accessible_name_required() const
{
    return true;
}

bool Link::children_are_presentational() const
{
    return false;
}

DefaultValueType Link::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Link::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& MenuItem::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(6);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& MenuItem::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(19);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaPosInSet);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaSetSize);

    }
    return properties;
}

HashTable<StateAndProperties> const& MenuItem::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& MenuItem::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& MenuItem::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& MenuItem::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& MenuItem::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(3);
        roles.set(Role::group);
        roles.set(Role::menu);
        roles.set(Role::menubar);

    }
    return roles;
}

HashTable<Role> const& MenuItem::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

MenuItem::MenuItem() { }

MenuItem::MenuItem(AriaData const& data)
    : Command(data)
{
}

bool MenuItem::accessible_name_required() const
{
    return true;
}

bool MenuItem::children_are_presentational() const
{
    return false;
}

DefaultValueType MenuItem::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource MenuItem::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& MenuItemCheckBox::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(6);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& MenuItemCheckBox::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(19);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaPosInSet);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaSetSize);

    }
    return properties;
}

HashTable<StateAndProperties> const& MenuItemCheckBox::required_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(1);
        states.set(StateAndProperties::AriaChecked);

    }
    return states;
}

HashTable<StateAndProperties> const& MenuItemCheckBox::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& MenuItemCheckBox::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& MenuItemCheckBox::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& MenuItemCheckBox::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(3);
        roles.set(Role::group);
        roles.set(Role::menu);
        roles.set(Role::menubar);

    }
    return roles;
}

HashTable<Role> const& MenuItemCheckBox::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

MenuItemCheckBox::MenuItemCheckBox() { }

MenuItemCheckBox::MenuItemCheckBox(AriaData const& data)
    : MenuItem(data)
{
}

bool MenuItemCheckBox::accessible_name_required() const
{
    return true;
}

bool MenuItemCheckBox::children_are_presentational() const
{
    return true;
}

DefaultValueType MenuItemCheckBox::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource MenuItemCheckBox::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& MenuItemRadio::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& MenuItemRadio::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(19);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaPosInSet);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaSetSize);

    }
    return properties;
}

HashTable<StateAndProperties> const& MenuItemRadio::required_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(1);
        states.set(StateAndProperties::AriaChecked);

    }
    return states;
}

HashTable<StateAndProperties> const& MenuItemRadio::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& MenuItemRadio::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& MenuItemRadio::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& MenuItemRadio::required_context_roles() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(3);
        roles.set(Role::group);
        roles.set(Role::menu);
        roles.set(Role::menubar);

    }
    return roles;
}

HashTable<Role> const& MenuItemRadio::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

MenuItemRadio::MenuItemRadio() { }

MenuItemRadio::MenuItemRadio(AriaData const& data)
    : MenuItemCheckBox(data)
{
}

bool MenuItemRadio::accessible_name_required() const
{
    return true;
}

bool MenuItemRadio::children_are_presentational() const
{
    return true;
}

DefaultValueType MenuItemRadio::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource MenuItemRadio::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& CheckBox::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(7);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaInvalid);

    }
    return states;
}

HashTable<StateAndProperties> const& CheckBox::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(19);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaReadOnly);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& CheckBox::required_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(1);
        states.set(StateAndProperties::AriaChecked);

    }
    return states;
}

HashTable<StateAndProperties> const& CheckBox::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& CheckBox::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& CheckBox::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& CheckBox::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& CheckBox::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

CheckBox::CheckBox() { }

CheckBox::CheckBox(AriaData const& data)
    : Input(data)
{
}

bool CheckBox::accessible_name_required() const
{
    return true;
}

bool CheckBox::children_are_presentational() const
{
    return true;
}

DefaultValueType CheckBox::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource CheckBox::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& ComboBox::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& ComboBox::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(22);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaAutoComplete);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaReadOnly);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& ComboBox::required_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(1);
        states.set(StateAndProperties::AriaExpanded);

    }
    return states;
}

HashTable<StateAndProperties> const& ComboBox::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& ComboBox::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& ComboBox::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& ComboBox::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& ComboBox::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

ComboBox::ComboBox() { }

ComboBox::ComboBox(AriaData const& data)
    : Input(data)
{
}

bool ComboBox::accessible_name_required() const
{
    return true;
}

bool ComboBox::children_are_presentational() const
{
    return false;
}

DefaultValueType ComboBox::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaHasPopup:
        return AriaHasPopup::Listbox;

    default:
        return {};
    }
}

NameFromSource ComboBox::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Radio::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& Radio::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(18);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaPosInSet);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);
        properties.set(StateAndProperties::AriaSetSize);

    }
    return properties;
}

HashTable<StateAndProperties> const& Radio::required_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(1);
        states.set(StateAndProperties::AriaChecked);

    }
    return states;
}

HashTable<StateAndProperties> const& Radio::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Radio::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Radio::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Radio::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Radio::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Radio::Radio() { }

Radio::Radio(AriaData const& data)
    : Input(data)
{
}

bool Radio::accessible_name_required() const
{
    return true;
}

bool Radio::children_are_presentational() const
{
    return true;
}

DefaultValueType Radio::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Radio::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& TextBox::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(6);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaInvalid);

    }
    return states;
}

HashTable<StateAndProperties> const& TextBox::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(24);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaAutoComplete);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaMultiLine);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaPlaceholder);
        properties.set(StateAndProperties::AriaReadOnly);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& TextBox::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& TextBox::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& TextBox::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& TextBox::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& TextBox::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& TextBox::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

TextBox::TextBox() { }

TextBox::TextBox(AriaData const& data)
    : Input(data)
{
}

bool TextBox::accessible_name_required() const
{
    return true;
}

bool TextBox::children_are_presentational() const
{
    return false;
}

DefaultValueType TextBox::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource TextBox::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& Switch::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(7);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaExpanded);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaInvalid);

    }
    return states;
}

HashTable<StateAndProperties> const& Switch::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(18);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& Switch::required_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(1);
        states.set(StateAndProperties::AriaChecked);

    }
    return states;
}

HashTable<StateAndProperties> const& Switch::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& Switch::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& Switch::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& Switch::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& Switch::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

Switch::Switch() { }

Switch::Switch(AriaData const& data)
    : CheckBox(data)
{
}

bool Switch::accessible_name_required() const
{
    return true;
}

bool Switch::children_are_presentational() const
{
    return true;
}

DefaultValueType Switch::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource Switch::name_from_source() const
{
    return NameFromSource::AuthorContent;
}

HashTable<StateAndProperties> const& SearchBox::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(6);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);
        states.set(StateAndProperties::AriaInvalid);

    }
    return states;
}

HashTable<StateAndProperties> const& SearchBox::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(24);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaAutoComplete);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaErrorMessage);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaHasPopup);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaMultiLine);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaPlaceholder);
        properties.set(StateAndProperties::AriaReadOnly);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRequired);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& SearchBox::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& SearchBox::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& SearchBox::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& SearchBox::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& SearchBox::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& SearchBox::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

SearchBox::SearchBox() { }

SearchBox::SearchBox(AriaData const& data)
    : TextBox(data)
{
}

bool SearchBox::accessible_name_required() const
{
    return true;
}

bool SearchBox::children_are_presentational() const
{
    return false;
}

DefaultValueType SearchBox::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource SearchBox::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& TabList::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(5);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaDisabled);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& TabList::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(19);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaMultiSelectable);
        properties.set(StateAndProperties::AriaOrientation);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& TabList::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& TabList::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& TabList::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& TabList::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& TabList::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& TabList::required_owned_elements() const
{
    static HashTable<Role> roles;
    if (roles.is_empty()) {
        roles.ensure_capacity(1);
        roles.set(Role::tab);

    }
    return roles;
}

TabList::TabList() { }

TabList::TabList(AriaData const& data)
    : Composite(data)
{
}

bool TabList::accessible_name_required() const
{
    return false;
}

bool TabList::children_are_presentational() const
{
    return false;
}

DefaultValueType TabList::default_value_for_property_or_state(StateAndProperties state_or_property) const
{
    switch (state_or_property) {

    case StateAndProperties::AriaOrientation:
        return AriaOrientation::Horizontal;

    default:
        return {};
    }
}

NameFromSource TabList::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& GraphicsDocument::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& GraphicsDocument::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& GraphicsDocument::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& GraphicsDocument::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& GraphicsDocument::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& GraphicsDocument::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& GraphicsDocument::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& GraphicsDocument::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

GraphicsDocument::GraphicsDocument() { }

GraphicsDocument::GraphicsDocument(AriaData const& data)
    : Document(data)
{
}

bool GraphicsDocument::accessible_name_required() const
{
    return true;
}

bool GraphicsDocument::children_are_presentational() const
{
    return false;
}

DefaultValueType GraphicsDocument::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource GraphicsDocument::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& GraphicsObject::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& GraphicsObject::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(17);
        properties.set(StateAndProperties::AriaActiveDescendant);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& GraphicsObject::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& GraphicsObject::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& GraphicsObject::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& GraphicsObject::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& GraphicsObject::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& GraphicsObject::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

GraphicsObject::GraphicsObject() { }

GraphicsObject::GraphicsObject(AriaData const& data)
    : Group(data)
{
}

bool GraphicsObject::accessible_name_required() const
{
    return false;
}

bool GraphicsObject::children_are_presentational() const
{
    return false;
}

DefaultValueType GraphicsObject::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource GraphicsObject::name_from_source() const
{
    return NameFromSource::Author;
}

HashTable<StateAndProperties> const& GraphicsSymbol::supported_states() const
{
    static HashTable<StateAndProperties> states;
    if (states.is_empty()) {
        states.ensure_capacity(4);
        states.set(StateAndProperties::AriaBusy);
        states.set(StateAndProperties::AriaCurrent);
        states.set(StateAndProperties::AriaGrabbed);
        states.set(StateAndProperties::AriaHidden);

    }
    return states;
}

HashTable<StateAndProperties> const& GraphicsSymbol::supported_properties() const
{
    static HashTable<StateAndProperties> properties;
    if (properties.is_empty()) {
        properties.ensure_capacity(16);
        properties.set(StateAndProperties::AriaAtomic);
        properties.set(StateAndProperties::AriaBrailleLabel);
        properties.set(StateAndProperties::AriaBrailleRoleDescription);
        properties.set(StateAndProperties::AriaControls);
        properties.set(StateAndProperties::AriaDescribedBy);
        properties.set(StateAndProperties::AriaDescription);
        properties.set(StateAndProperties::AriaDetails);
        properties.set(StateAndProperties::AriaDropEffect);
        properties.set(StateAndProperties::AriaFlowTo);
        properties.set(StateAndProperties::AriaKeyShortcuts);
        properties.set(StateAndProperties::AriaLabel);
        properties.set(StateAndProperties::AriaLabelledBy);
        properties.set(StateAndProperties::AriaLive);
        properties.set(StateAndProperties::AriaOwns);
        properties.set(StateAndProperties::AriaRelevant);
        properties.set(StateAndProperties::AriaRoleDescription);

    }
    return properties;
}

HashTable<StateAndProperties> const& GraphicsSymbol::required_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& GraphicsSymbol::required_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<StateAndProperties> const& GraphicsSymbol::prohibited_states() const
{
    static HashTable<StateAndProperties> states;
    return states;
}

HashTable<StateAndProperties> const& GraphicsSymbol::prohibited_properties() const
{
    static HashTable<StateAndProperties> properties;
    return properties;
}

HashTable<Role> const& GraphicsSymbol::required_context_roles() const
{
    static HashTable<Role> roles;
    return roles;
}

HashTable<Role> const& GraphicsSymbol::required_owned_elements() const
{
    static HashTable<Role> roles;
    return roles;
}

GraphicsSymbol::GraphicsSymbol() { }

GraphicsSymbol::GraphicsSymbol(AriaData const& data)
    : Img(data)
{
}

bool GraphicsSymbol::accessible_name_required() const
{
    return true;
}

bool GraphicsSymbol::children_are_presentational() const
{
    return true;
}

DefaultValueType GraphicsSymbol::default_value_for_property_or_state(StateAndProperties) const
{
    return {};
}

NameFromSource GraphicsSymbol::name_from_source() const
{
    return NameFromSource::Author;
}
}