<?php
/**
 * Created by PhpStorm.
 * User: mateusz
 * Date: 18.07.17
 * Time: 12:48
 */
/**
 * Adds together two matrices of same dimensions
 *
 * @param $a array matrix
 * @param $b array matrix
 * @return array matrix
 */
function addMatrices($a, $b)
{
    # Uzupełnić, najlepiej pętlami dwoma
    $c = [];
    for ($i = 0; $i < 2; $i++) {
        for ($j = 0; $j < 3; $j++) {
            $c [$i][$j] = $a[$i][$j] + $b [$i][$j];
        }
    }
    return $c;
}

$matrixA = [
    [0, 1, 2],
    [9, 8, 7]
];


$matrixB = [
    [6, 5, 4],
    [3, 4, 5]
];

$expectedResult = [
    [6, 6, 6],
    [12, 12, 12]
];


$answer = addMatrices($matrixA, $matrixB);

# Testy

if (!is_array($answer)) {
    echo 'zmienna $answer nie jest tablicą, a musi.' . PHP_EOL;
    die(1);
}

if (count($answer) != count($expectedResult)) {
    echo "Niepoprawny rozmiar tablicy, wymagane: " . count($expectedResult) . " jest: " . count($answer) . PHP_EOL;
    die(1);
}

foreach ($answer as $i => $item) {
    if (count($item) == 3) continue;
    echo "Nieprawidłowy rozmiar, wymagane " . count($expectedResult[$i]) . ", jest: " . count($item) . PHP_EOL;
    die();
}

echo printf("[%s] = [%s]", implode(", ", $expectedResult[0]), implode(", ", $answer[0])) . PHP_EOL;
echo printf("[%s] = [%s]", implode(", ", $expectedResult[1]), implode(", ", $answer[0])) . PHP_EOL;


if ($expectedResult != $answer) {
    echo "Odpowiedź nie jest poprawna :|" . PHP_EOL;
    die(1);
}
echo "Odpowiedź JEST poprawna!" . PHP_EOL;