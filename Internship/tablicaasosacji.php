<?php
/**
 * Created by PhpStorm.
 * User: mateusz
 * Date: 13.07.17
 * Time: 12:34
 */
if (!isset ($argv[1])) {
    echo 'Please type a country name' . PHP_EOL;
    exit (1);
}
$countries = [
    "Italy" => "Rome",
    "Luxembourg" => "Luxembourg",
    "Belgium" => "Brussels",
    "Denmark" => "Copenhagen",
    "Finland" => "Helsinki",
    "France" => "Paris",
    "Slovakia" => "Bratislava",
    "Slovenia" => "Ljubljana",
    "Germany" => "Berlin",
    "Greece" => "Athens",
    "Ireland" => "Dublin",
    "Netherlands" => "Amsterdam",
    "Portugal" => "Lisbon",
    "Spain" => "Madrid",
    "Sweden" => "Stockholm",
    "United Kingdom" => "London",
    "Cyprus" => "Nicosia",
    "Lithuania" => "Vilnius",
    "Czech Republic" => "Prague",
    "Estonia" => "Tallin",
    "Hungary" => "Budapest",
    "Latvia" => "Riga",
    "Malta" => "Valetta",
    "Austria" => "Vienna",
    "Poland" => "Warsaw",
    "Vatican" => "Vatican",
];
$country = $argv[1];
$country = ucfirst($country);
if (array_key_exists($country, $countries)) {
    $capital = $countries[$country];
    echo "The capital of $country is $capital" . PHP_EOL;
    if ($capital == $country) {
        echo "$country is a city state" . PHP_EOL;
    }
} else {
    echo 'The capital is unknow' . PHP_EOL;
}