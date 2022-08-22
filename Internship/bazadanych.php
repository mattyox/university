<?php
/**
 * Created by PhpStorm.
 * User: mateusz
 * Date: 24.07.17
 * Time: 22:14
 */
$presentYear = date('Y');
$data = file_get_contents('dane.txt');
$line = explode("\n", $data);
for ($i = 0; $i <= 4; $i++) {
    $personData = explode(';', $line[$i]);
    $name = $personData[0];
    $surname = $personData[1];
    $bornYear = $personData[2];
    $bornPlace = $personData[3];
    $age = $presentYear - $bornYear;
    echo "$name $surname urodzony w $bornPlace w roku $bornYear obecnie ma $age lat ". PHP_EOL;
}