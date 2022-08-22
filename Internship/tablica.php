<?php
/**
 * Created by PhpStorm.
 * User: mateusz
 * Date: 12.07.17
 * Time: 10:54
 */
$a = [
    2,
    4,
    6,
    8,
    10
];
$wynik = 0;
foreach ($a as $el) {
    $wynik = $wynik + $el;
}
echo "Wynik:$wynik" . PHP_EOL;