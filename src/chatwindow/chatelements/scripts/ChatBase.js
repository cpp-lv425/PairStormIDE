.pragma library

const golden_ratio    = 1.61803398875
const gWhiteThemeName = "white"
const gBlueThemeName  = "blue"
const gDarkThemeName  = "dark"

function backgroundColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#dee7ee";
    case gBlueThemeName:
        return "#576978";
    case gDarkThemeName:
        return "#252d33";
    default:
        return "#dee7ee";
    }
}

function backgroundTextColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#748ca0";
    case gBlueThemeName:
        return "#b3bfc9";
    case gDarkThemeName:
        return "#b3bfc9";
    default:
        return "#748ca0";
    }
}

function chatUserMessageColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#436697";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function chatAuthorMessageColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#4e6072";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function chatSystemMessageColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#438497";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function chatMessageTextColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#eaf4ff";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function chatInputBackgroundColor(themeName)
{

    switch(themeName)
    {
    case gWhiteThemeName:
        return "#c3d5e8";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function chatInputTextColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#1e3956";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function chatInputSelectionBackColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#1e262d";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function chatInputSelectionTextColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#1e262d";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function chatInputTextHintColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#7f8e9d";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function chatUsersBackgroundColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#bdd3ed";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function chatUsersTextColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#516680";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function chatUsersSpacerColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#7798c0";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function switchTurnedOnColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#68afa5";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}

function switchTurnedOffColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#e5f0f8";
    case gDarkThemeName:
        return "#b3bfc9";
    }
}
