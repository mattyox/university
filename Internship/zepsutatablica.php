<?php
/**
 * Created by PhpStorm.
 * User: mateusz
 * Date: 12.07.17
 * Time: 12:22
 */
$brokenNames = [
    "Adam",
    "Karol",
    2,
    "Karolina",
    "Abdul",
    1,
    "Mateusz",
    "Karyna",
    3,
    "Janusz",
    "Grażyna",
    "Sebstian",
    7,
    "Dżesika",
    "Wiktor",
    "Benedykt"
];
$cyfry = [];
$imiona = [];
$imionaPrint = 0;
$cyfryPrint = 0;
foreach ($brokenNames as $name) {
    if (is_numeric($name)) {
        $cyfry[] = $name;
    } else {
        $imiona[] = $name;
    }
}
$imionaPrint = implode(", ", $imiona);
$cyfryPrint = implode("", $cyfry);
echo 'Imiona: ' . $imionaPrint . PHP_EOL;
echo 'Cyfry: ' . $cyfryPrint . PHP_EOL;
$first = substr($cyfryPrint, 0, 1);
$last = substr($cyfryPrint, 3, 1);
echo $first.$last . PHP_EOL;
$firstTwo = substr($cyfryPrint, 0, 2);
$lastTwo = substr($cyfryPrint, 2, 2);
echo $firstTwo.':'.$lastTwo . PHP_EOL;