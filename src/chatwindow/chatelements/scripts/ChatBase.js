.pragma library

const goldenRatio     = 1.61803398875
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
    case gBlueThemeName:
        return "#2692c8";
    case gDarkThemeName:
        return "#6193b0";
    default:
        return "#436697";
    }
}

function chatUserMessageDateColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#8aafe2";
    case gBlueThemeName:
        return "#5db8e6";
    case gDarkThemeName:
        return "#2d556c";
    default:
        return "#8aafe2";
    }
}

function chatAuthorMessageColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#4e6072";
    case gBlueThemeName:
        return "#2482d2";
    case gDarkThemeName:
        return "#659bc3";
    default:
        return "#4e6072";
    }
}

function chatAuthorMessageDateColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#88a2bc";
    case gBlueThemeName:
        return "#6bb2ef";
    case gDarkThemeName:
        return "#305f80";
    default:
        return "#88a2bc";
    }
}

function chatSystemMessageColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#438497";
    case gBlueThemeName:
        return "#63952d";
    case gDarkThemeName:
        return "#65b0c3";
    default:
        return "#438497";
    }
}

function chatSystemMessageDateColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#7ec5d9";
    case gBlueThemeName:
        return "#91bf60";
    case gDarkThemeName:
        return "#3b7482";
    default:
        return "#7ec5d9";
    }
}

function chatMessageTextColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#eaf4ff";
    case gBlueThemeName:
        return "#eaf4ff";
    case gDarkThemeName:
        return "#092b45";
    default:
        return "#eaf4ff";
    }
}

function chatInputBackgroundColor(themeName)
{

    switch(themeName)
    {
    case gWhiteThemeName:
        return "#c3d5e8";
    case gBlueThemeName:
        return "#29567c";
    case gDarkThemeName:
        return "#315e7f";
    default:
        return "#c3d5e8";
    }
}

function chatInputTextColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#1e3956";
    case gBlueThemeName:
        return "#e0f2ff";
    case gDarkThemeName:
        return "#e0f2ff";
    default:
        return "#1e3956";
    }
}

function chatInputSelectionBackColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#637d99";
    case gBlueThemeName:
        return "#748696";
    case gDarkThemeName:
        return "#1179c6";
    default:
        return "#637d99";
    }
}

function chatInputSelectionTextColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#b4d5f9";
    case gBlueThemeName:
        return "#e0f2ff";
    case gDarkThemeName:
        return "#e0f2ff";
    default:
        return "#b4d5f9";
    }
}

function chatInputTextHintColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#7f8e9d";
    case gBlueThemeName:
        return "#7da7ca";
    case gDarkThemeName:
        return "#6c9abc";
    default:
        return "#7f8e9d";
    }
}

function chatUsersBackgroundColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#bdd3ed";
    case gBlueThemeName:
        return "#73aecc";
    case gDarkThemeName:
        return "#1f4d6f";
    default:
        return "#bdd3ed";
    }
}

function chatUsersTextColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#516680";
    case gBlueThemeName:
        return "#0e3245";
    case gDarkThemeName:
        return "#d1ecff";
    default:
        return "#516680";
    }
}

function chatUsersSpacerColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#7798c0";
    case gBlueThemeName:
        return "#5a96b4";
    case gDarkThemeName:
        return "#37709a";
    default:
        return "#7798c0";
    }
}

function switchTurnedOnColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#68afa5";
    case gBlueThemeName:
        return "#61e381";
    case gDarkThemeName:
        return "#2fb098";
    default:
        return "#68afa5";
    }
}

function switchTurnedOffColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#e5f0f8";
    case gBlueThemeName:
        return "#e5f0f8";
    case gDarkThemeName:
        return "#aedcff";
    default:
        return "#e5f0f8";
    }
}
