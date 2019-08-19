.pragma library

function sleep_for(delay)
{
    var start = new Date().getTime();
    while (new Date().getTime() < start + delay);
}

var golden_ratio = 1.61803398875
var gray = "#b2b1b1"
var lightGray = "#dddddd"
var light = "#ffffff"
var blue = "#2d548b"
var mainColor = "#17a81a"
var dark = "#222222"
var mainColorDarker = Qt.darker(mainColor, 1.5)
var baseSize = 10
var smallSize = 10
var largeSize = 16

function golder_ratio_times(multiplier)
{
    return golden_ratio * multiplier
}
