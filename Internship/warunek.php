<?php
/**
 * Created by PhpStorm.
 * User: mateusz
 * Date: 06.07.17
 * Time: 13:46
 */
if (!isset ($argv[1])&&!isset ($argv[2]))
{
    echo 'Proszę podać parametry' .PHP_EOL;
    exit (1);
}
$a=$argv[1];
$b=$argv[2];
echo "$a+$b=".($a+$b) .PHP_EOL;
echo "$a-$b=".($a-$b) .PHP_EOL;
echo "$a*$b=".($a*$b) .PHP_EOL;
if ($b!=0)
{
    echo "$a/$b=" . ($a / $b) .PHP_EOL;
}
else
{
    echo 'Nie można wykonać opercji' .PHP_EOL;
}