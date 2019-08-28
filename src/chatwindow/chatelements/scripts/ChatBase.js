.pragma library

// TODO: remove global theme variable & replace corresponding statements
const globalTheme = "white"

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
        return "#0f2c49";
    }
}

function chatAuthorMessageColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#4e6072";
    }
}

function chatSystemMessageColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#285852";
    }
}

function chatMessageTextColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#eef2f6";
    }
}

function chatInputBackgroundColor(themeName)
{
    return Qt.darker(backgroundColor(themeName), 1.1)
    /*
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#8da9bf";
    }*/
}

function chatInputTextColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#1e262d";
    }
}

function chatInputSelectionBackColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#1e262d";
    }
}

function chatInputSelectionTextColor(themeName)
{
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#1e262d";
    }
}

function chatInputTextHintColor(themeName)
{
    return backgroundColor(themeName);
    /*
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#1e262d";
    }*/
}

function chatUsersBackgroundColor(themeName)
{
    return Qt.darker(backgroundColor(themeName), 1.1)
    /*
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#8da9bf";
    }*/
}

function chatUsersTextColor(themeName)
{
    return "white";//Qt.darker(chatTextColor(themeName), 1.1)
    /*
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#8da9bf";
    }*/
}

function chatUsersSpacerColor(themeName)
{
    return Qt.darker(backgroundColor(themeName), 1.3)
    /*
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#8da9bf";
    }*/
}

function switchTurnedOnColor(themeName)
{
    return Qt.darker(backgroundColor(themeName), 1.3)
    /*
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#8da9bf";
    }*/
}

function switchTurnedOffColor(themeName)
{
    return Qt.darker(backgroundColor(themeName), 1.3)
    /*
    switch(themeName)
    {
    case gWhiteThemeName:
        return "#8da9bf";
    }*/
}
