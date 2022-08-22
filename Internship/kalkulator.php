<?php
/**
 * Created by PhpStorm.
 * User: mateusz
 * Date: 11.07.17
 * Time: 11:03
 */
if (isset ($argv[1]) && isset ($argv[2]) && $argv[2] !== "!") {
    if (!isset ($argv[3])) {
        echo 'Proszę podać parametry' . PHP_EOL;
        exit (1);
    }
}
if (!isset($argv[3])) {
    $argv[3] = 0;
}
$a = $argv[1];
$b = $argv[2];
$c = $argv[3];
function root($x, $y)
{
    return pow($x, (1 / $y));
}

function fac($x)
{
    $w = 1;
    for ($i = 1; $i <= $x; $i++) {
        $w = $w * $i;
    }
    return $w;
}

switch ($b) {
    case"+":
        echo "$a $b $c = " . ($a + $c) . PHP_EOL;
        break;
    case"-":
        echo "$a $b $c = " . ($a - $c) . PHP_EOL;
        break;
    case"*":
    case"x":
        echo "$a $b $c = " . ($a * $c) . PHP_EOL;
        break;
    case"/":
        if ($c == 0) {
            echo "Nie można wykonać operaji" . PHP_EOL;
            exit(1);
        }
        echo "$a $b $c = " . ($a / $c) . PHP_EOL;
        break;
    case"^":
        echo "$a $b $c = " . pow($a, $c) . PHP_EOL;
        break;
    case"r":
        echo "$a $b $c = " . root($a, $c) . PHP_EOL;
        break;
    case"!":
        echo "$a $b = " . fac($a) . PHP_EOL;
}