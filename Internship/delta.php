<?php

$a = $argv[1];
$b = $argv[2];
$c = $argv[3];
$d = $argv[4];

$delta = 0;
$x = 0;
$x1 = 0;
$x2 = 0;

$delta = (pow($b, 2)) - (4 * $a * $c);

echo 'Delta = ' . $delta . PHP_EOL;

switch ($d) {
    case ($d = "="):
        switch ($delta) {
            case ($delta < 0):
                echo 'Brak rozwiązania' . PHP_EOL;
                break;
            case ($delta == 0):
                $x = ((-$b) / 2);
                echo 'Jedno rozwiązanie' . PHP_EOL;
                echo "x = $x" . PHP_EOL;
                break;
            case ($delta > 0):
                $x1 = ((-$b) + $delta) / 2;
                $x2 = ((-$b) - $delta) / 2;
                echo 'Dwa rozwiązania' . PHP_EOL;
                echo "x1 = $x1" . PHP_EOL;
                echo "x2 = $x2" . PHP_EOL;
                break;
        }
    case ($d = "<"):
}