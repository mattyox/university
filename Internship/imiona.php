<?php
/**
 * Created by PhpStorm.
 * User: mateusz
 * Date: 12.07.17
 * Time: 11:14
 */
$names =
    [
        "Adam",
        "Karol",
        "Karolina",
        "Abdul",
        "Mateusz",
        "Karyna",
        "Janusz",
        "Grażyna",
        "Sebastian",
        "Dżesika"
    ];
$name = 0;
foreach ($names as $name) {
    if (substr($name, -1) == 'a') {
        echo $name.' Dziewczyna' . PHP_EOL;
    }
    else {
        echo $name.' Chłopak' . PHP_EOL;
    }
}