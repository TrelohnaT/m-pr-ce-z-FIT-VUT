# Implementační dokumentace k 1. úloze do IPP 2019/2020
### jméno a příjmení:  Václav Doleček 
### Login: xdolec03
## Cíl projektu
- Cíl projektu bylo napsat parse, který bude chopen přeložit code IPPcode20 do validního XML. Jako programovací jazyk bylo zadáno PHP 7.4.
## Řešení projektu a postupy při implementaci
### Použité funkce
#### regex_var($string)
- $string - type string
- Funkce porovná předaný string s regulárním výrazem
- projde-li, jedná se o typ var a je vráceno True, jinak False
#### print_var($string, $count_var)
- $string - type string
- $count_var - type int
- Funkce korektně vytiskne argument a jeho pořadí na stdout v XML kódu.
#### regex_symb($string)
- $string - type string
- Funkce porovná předaný strings regulárním výrazem
- projde-li, jedná se o typ symb a je vráceno True, jinak False
#### print_symb($string, $count_var)
- $string - type string
- $count_var - type int
-  Funkce korektně vytiskne argument a jeho pořadí na stdout v XML kódu.
#### regex_label($string)
- $string - type string
- Funkce porovná předaný strings regulárním výrazem
- projde-li, jedná se o typ label a je vráceno True, jinak False
#### print_label($string, $count_var)
- $string - type string
- $count_var - type int
-  Funkce korektně vytiskne argument a jeho pořadí na stdout v XML kódu.
#### regex_type($string)
- $string - type string
- Funkce porovná předaný strings regulárním výrazem
- projde-li, jedná se o typ type a je vráceno True, jinak False
#### print_type($string, $count_var)
- $string - type string
- $count_var - type int
-  Funkce korektně vytiskne argument a jeho pořadí na stdout v XML kódu.
#### instruction_begin($count_print_intsr, $kind_print_instr)
- $count_print_intsr - pořadí tisknuté instrukce
- $kind_print_instr - druh tisknuté instrukce
- Funkce vytiskne začátek (hlavičku) instrukce spolu s pořadím instrukce a druhem instrukce
#### instruction_end()
- Funkce vytiskne konec právě vytisknuté instrukce

### Krátce o implementaci
- Funkce jsou deklarovány hned na začátku. Program samotný začíná na řádku 285.
- Jako první jsou zpracovány případné argumenty, se kterými by mohl být script spuštěn. Nenajde se-li chyba nekorektní argument, vytiskne se "help" a nebo se pokračuje zpracovávání IPPcode20.
- Jako první se vše nahraje do proměnné $input_file, zde se vytáhne první řádek pro kontrolu hlavičky. Je-li nekorektní, vytáhneme další řádek. To dokud nenajdeme odpovídající hlavičku a nebo nám nedojdou řádky.
- Projde-li vše v pořádku a hlavička je nalezena, vytiskneme hlavičku XML na stdout, spočítáme řádky vstupního kódu.
- Každý řádek se nahraje do proměnné $actual_line kde se rozštěpí podle mezer (" ") na jednotlivá slova do proměnné $words
>$words[0] -> klíčové slovo instrukce
>$words[0 - 3] -> parametry instrukce
- Najdeme o kterou instrukci se jedná, ověříme, zda její parametry jsou správného typu a zavoláme patřičné funkce které tuto instrukci vytisknou na stdout
- Po projetí všech řádku, se vytiskne ukončení XML a script skončí
- Nastane-li kdykoliv v běhu programu chyba, script skončí po vrácení patřičného číslo pro tuto chybu

## Spuštění
- php 7.4 parse.php [argumenty]
- doporučujeme script "krmit" ze souboru pomocí přesměrovaného stdin (např. php 7.4 parse.php <test.txt)
### Podporované argumenty
- -h
- --help
- --stats
- --loc --stats
- --commets --stats
- --labels --stats
- --jumps --stats

## Zdroje a odkazy
- Z těchto stránek jsem načerpal potřebné informace k řešení tohoto projektu:
[https://www.w3schools.com/php/](https://www.w3schools.com/php/)
[https://www.php.net/manual/en/function.preg-match.php](https://www.php.net/manual/en/function.preg-match.php)
[https://www.phpliveregex.com/](https://www.phpliveregex.com/)