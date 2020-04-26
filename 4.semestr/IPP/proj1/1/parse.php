<?php
#Projekt první do předmětu IPP
#Autor: Václav Doleček
#login: xdolec03

#TO DO LIST:
#1) dopsat něco smysluplného do help
#2) 
#3) opravit všecky bugy
#
#
#
#42) shrábnout body a udělat tento předmět

#poznámky pro prográmatora:
#nějak jsem nepostřehnul jak toto napsat Objektově Orientovaně
#proto to píšu tak jak umím a tak aby to fungovalo...
#...což bych řekl, že je taky cílem, aby to fungovalo

#chybové návratové hodnoty
$error_parametr_script = 10;
$error_input_file_open = 11;
$error_output_file_open = 12;
$error_inter = 99;

$error_missing_head = 21;
$error_in_ippcode20 = 22;
$error_lexikal_or_syntact = 23;

$nonerror = 0;

#Zde jsou předpisy funkci pro porovnávání stringu s regulárními vyrazy
#hned pod funkci pro regex datového typu je funkce pro jeho korektní
#tisknutí. Nejpre jsem to měl všecko v jednom, ale to dělalo neplechu,
#takže jsem se to rozhodl rozdělit

#funkce pro kontrolu var s regexem
function regex_var($string)
{
    if(preg_match('/(GF@|LF@|TF@)(\S*|[a-zA-Z]+[0-9]*)+/', $string) == TRUE)
    {
        return(TRUE);
    }
    return(FALSE);
}
#funkce pro tiskt var
#1. argument je nazev proměnné samotné
#2. argument je pořadí této instrukce
function print_var($string, $count_var)
{
    echo "\t\t<arg".$count_var." type=\"var\">".$string."</arg".$count_var.">\n";
    return(TRUE);
}
#funkce pro kontrolu symb s regexem
function regex_symb($string)
{
    #pokud najdu int
    if(preg_match('/int@-?[0-9]+/i', $string) == TRUE)
    {
        return(TRUE);
    }
    #pokud je symb typu string
    else if(preg_match('/string@(.*|[a-zA-Z]*|.*)+/i', $string) == TRUE)
    {
        return(TRUE);
    }
    #pokud je symb konstanta
    else if(regex_var($string) == TRUE)
    {
        return(TRUE);
    }
    #pokud je symb typu bool
    else if(preg_match('/bool@(TRUE|FALSE)/i', $string) == TRUE)
    {
        return(TRUE);
    }
    #pokud je symb typu nil
    else if(preg_match('/nil@(.*|[a-zA-Z]*|.*)+/i', $string) == TRUE)
    {
        return(TRUE);
    }
    #pokud je cokoliv jiného
    else
    {
        return(FALSE);
    }
}
#funkce pro korektní tisk symb
#1. argument je nazev proměnné samotné
#2. argument je pořadí této instrukce
function print_symb($string, $count_symb)
{
    #pokud najdu int
    if(preg_match('/int@-?[0-9]+/i', $string) == TRUE)
    {
        $string = explode("@", $string);
        echo "\t\t<arg".$count_symb." type=\"int\">".$string[1]."</arg".$count_symb.">\n";
        return(TRUE);
    }
    #pokud je symb typu string
    else if(preg_match('/string@(.*|[a-zA-Z]*|.*)+/i', $string) == TRUE)
    {
        $string = explode("@", $string);
        $string_right_side = $string[1];
        $string_count = strlen($string_right_side);
        $string_right_side = str_split($string_right_side, 1);
        echo "\t\t<arg1 type=\"string\">";
        for($a = 0; $a <= $string_count; $a++)
        {
            switch ($string_right_side[$a]) 
            {
                case "<":
                    echo "&lt;";    
                break;

                case ">":
                    echo "&gt;";
                break;

                case "&":
                    echo "&amp";
                break;

                case "\"":
                    echo "&quot";
                break;

                case "'":
                    echo "&apos";
                break;
                
                default:
                    echo $string_right_side[$a];
                break;
            }
        }
        echo "</arg".$count_symb.">\n";
        return(TRUE);
    }
    #pokud je symb konstanta
    else if(regex_var($string) == TRUE)
    {
        echo "\t\t<arg".$count_symb." type=\"var\">".$string."</arg".$count_symb.">\n";
        return(TRUE);
    }
    #pokud je symb typu bool
    else if(preg_match('/bool@(TRUE|FALSE)/i', $string) == TRUE)
    {
        $string = explode("@", $string);
        $string_right_side = $string[1];
        $string_count = strlen($string_right_side);
        $string_right_side = str_split($string_right_side, 1);
        if($string_right_side[0] == "t")
        {
            if($string_right_side[1] == "r")
            {
                if($string_right_side[2] == "u")
                {
                    if($string_right_side[3] == "e")
                    {
                        echo "\t\t<arg".$count_symb." type=\"bool\">".$string[1]."</arg".$count_symb.">\n";
                        return(TRUE);
                    }
                    else
                    {
                        return(FALSE);
                    }
                }
                else
                {
                    return(FALSE);
                }
            }
            else
            {
                return(FALSE);
            }
        }
        else if($string_right_side[0] == "f")
        {
            if($string_right_side[1] == "a")
            {
                if($string_right_side[2] == "l")
                {
                    if($string_right_side[3] == "s")
                    {
                        if($string_right_side[4] == "e")
                        {
                            echo "\t\t<arg".$count_symb." type=\"bool\">".$string[1]."</arg".$count_symb.">\n";
                            return(TRUE);
                        }
                        else
                        {
                            return(FALSE);
                        }
                    }
                    else
                    {
                        return(FALSE);
                    }
                }
                else
                {
                    return(FALSE);
                }
            }
            else
            {
                return(FALSE);
            }
        }
        return(TRUE);
    }
    #pokud je symb typu nil
    else if(preg_match('/nil@(.*|[a-zA-Z]*|.*)+/i', $string) == TRUE)
    {
        echo "\t\t<arg".$count_symb." type=\"nil\">nil</arg".$count_symb.">\n";
        return(TRUE);
    }
    #pokud je cokoliv jiného
    else
    {
        return(FALSE);
    }
}
#funkce pro kontrolu label s regexem
function regex_label($string)
{
    if(preg_match('/(\S*|[a-zA-Z]+|[0-9]*)+/', $string) == TRUE)
    {
        return(TRUE);
    }
    return(FALSE);
}
#funkce pro korektní tisk label
#1. argument je nazev proměnné samotné
#2. argument je pořadí této instrukce
function print_label($string, $cout_label)
{
    $string = explode("@", $string);
    echo "\t\t<arg".$count_label." type=\"label\">".$string[1]."</arg".$count_label.">\n";
    return(TRUE); 
}
#fukce pro kontrolu typu, typ je tisknut automaticky ve funkcích nad touto
function regex_type($string)
{
    $type_of_this = explode("@", $string);
    $type_of_this = strtoupper($type_of_this[0]);
    if(strcmp($type_of_this, "INT") == 0)
    {
        return(TRUE);
    }
    else if(strcmp($type_of_this, "STRING") == 0)
    {
        return(TRUE);
    }
    else if(strcmp($type_of_this, "NIL") == 0)
    {
        return(TRUE);
    }
    else if(strcmp($type_of_this, "BOOL") == 0)
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

function print_type($string, $cout_type)
{
    $string = explode("@", $string);
    echo "\t\t<arg".$count_type." type=\"type\">".$string[1]."</arg".$count_type.">\n";
    return(TRUE);
}

#pomocné funkce pro tisknutí
#protože jsem líny
#vytiskne začátek instrukce 1.argument je pořadí instrukce 2.argument je datový tym
function instruction_begin($count_print_instr, $kind_print_instr)
{
    echo "\t<instruction order=\"".($count_print_instr-1)."\" opcode=\"".$kind_print_instr."\">\n";
    return(TRUE);
}
#toto vytiskne konec instrukce
function instruction_end()
{
    echo "\t</instruction>\n";
    return(TRUE);
}

#getopt parametry ze vstupu a nahraje je do pole argument
$argument = getopt("h", ["help", "stats", "loc", "comments", "labels", "jumps"]);

#toto kontroluje s kolika argumenty byl zavolán program
if($argc == 2)
{
    #zde kontroluji, zda mi přilo na vztup to co mělo, což je -h nebo --help
    if(array_key_exists('h', $argument) || array_key_exists('help', $argument))
    {
        echo "\nAbout script parse.php \n";
        echo "\tScript of filtr type (parse.php made in PHP 7.4) will read IPPcode20\n";
        echo "\tfrom stdin, then will check lexical and syntact correctness. After that\n";
        echo "\twill ride down XML code at stdout.\n";
        echo "\nParametres\n";
        echo "\t--help -> ride down information about this script\n";
        echo "\t-h -> ride down information about this script\n";
        echo "\tstdin -> here must be ride down code which you want to parse\n";
        echo "\nIn case of any question, please contact me by e-mail, or by FB\n";
        echo "\tautor: Václav Dolecek (xdolec03)\n";
        fwrite(STDERR, $nonerror);
        exit($nonerror);
    }
    else if(array_key_exists('stats', $argument))
    {
        fwrite(STDERR, $nonerror);
        exit($nonerror);
    }
    #pokud mi příjde něco co neznám, tak vyhodím chybu
    else
    {
        echo "Some parametr doesn´t match what was expected\n";
        fwrite(STDERR, $error_parametr_script);
        exit($error_parametr_script);
    }
}
else if($argc == 1)
{
    //nic nedělej
}
else if($argc == 3)
{
    if(array_key_exists('loc', $argument))
    {
        if(array_key_exists('stats', $argument))
        {
            fwrite(STDERR, $nonerror);
            exit($nonerror);
        }
        else
        {
            echo "error\n";
            exit($error_parametr_script);
        }
    }
    else if(array_key_exists('comments', $argument))
    {
        if(array_key_exists('stats', $argument))
        {
            fwrite(STDERR, $nonerror);
            exit($nonerror);
        }
        else
        {
            echo "error\n";
            exit($error_parametr_script);
        }
    }
    else if(array_key_exists('labels', $argument))
    {
        if(array_key_exists('stats', $argument))
        {
            fwrite(STDERR, $nonerror);
            exit($nonerror);
        }
        else
        {
            echo "error\n";
            exit($error_parametr_script);
        }
    }
    else if(array_key_exists('jumps', $argument))
    {
        if(array_key_exists('stats', $argument))
        {
            fwrite(STDERR, $nonerror);
            exit($nonerror);
        }
        else
        {
            echo "error\n";
            exit($error_parametr_script);
        }
    }
}
else
{
    echo "Parameters doe not match. Be gone at once\n";
    fwrite(STDERR, $error_parametr_script);
    exit($error_parametr_script);
}

#toto mi nahraje celý obsah souboru do promněné input_file
if(($input_file = file("php://stdin")) == FALSE)
{
    echo "Problem with file\n";
    fwrite(STDERR, $error_input_file_open);
    exit($error_input_file_open);
}

#zde načtu první řádek pro kontrolu
$first_line = $input_file[0];

#zde kontroluji, zda první řádek sedí
$first_line = strtoupper($first_line);                  //toto to dá všecko na velká pismena
$first_line = explode(" ", $first_line);
$first_line[0] = str_replace(" ", "", $first_line[0]);        //toto tomu smaže všecky mezery
$first_line[0] = str_replace("\n", "", $first_line[0]);       //toto tomu smaže znak nového řádku
$first_line[0] = str_replace("\r", "", $first_line[0]);       //toto taky smaže znak nového řádku

#pokud je před hlavičkou komentář, toto ho bude ignorovat dokud nenajde hlavičku
for($line_with_head = 0; $first_line[0] !== '.IPPCODE20';)
{
    $first_line[0] = $input_file[$line_with_head];
    $first_line[0] = strtoupper($first_line[0]);
    $first_line[0] = str_replace("\r", "", $first_line[0]);
    $first_line[0] = str_replace("\n", "", $first_line[0]);
    if((preg_match('/#.*/', $first_line[0])) != TRUE)
    {
        break;        
    }
    $line_with_head++;
}


#zde se porovnává, zda je první řádek v pohodě
if($first_line[0] === '.IPPCODE20')
{
    #tisk hlavičky
    echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    echo "<program language=\"IPPcode20\">\n";
    #spočítám si kolik má soubour řádku
    $input_number_of_line = count($input_file);
    #deklarace čísla řádku, odečítá se počet zahozených řádku s komentářem
    $instcruction_line = (1-$line_with_head);
    #toto je hlavní for pro zpracovávání řádku, provede se tolikrát, kolik je řádku
    for($i = 1; $i <= $input_number_of_line; $i++)
    {
        #inkrementace čísla řádku
        $instcruction_line++;
        #nahraje se do proměnné aktualní zpracovávaný řádek
        $actual_line = $input_file[$i];
        
        #zde se řádek rozloží na jednotlivá slova
        $words = explode(" ", $actual_line);
        
        #spočítá se kolik je na řádku slov
        $actual_line_number_of_line = count($words);
        
        $words = str_replace("\r", "", $words);       //toto taky smaže znak nového řádku
        $words = str_replace("\n", "", $words);       //toto tomu smaže znak nového řádku
            
        $words[0] = strtoupper($words[0]);
            
        #zde se rozeznává, co je to za slovo
        switch($words[0])
        {
            case "MOVE":                //<var> <symb>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        instruction_begin($instcruction_line, $words[0]);
                        print_var($words[1], 1);
                        print_symb($words[2], 2);
                        instruction_end();
                        break;
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
            
            case "DEFVAR":              //<var>
                if(regex_var($words[1]) == TRUE)
                {
                    instruction_begin($instcruction_line, $words[0]);
                    print_var($words[1], 1);
                    instruction_end();
                    break;
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "CREATEFRAME":         
                instruction_begin($instcruction_line, $words[0]);
                instruction_end();
            break;
                
            case "PUSHFRAME":
                instruction_begin($instcruction_line, $words[0]);
                instruction_end();
            break;
                
            case "POPFRAME":
                instruction_begin($instcruction_line, $words[0]);
                instruction_end();
            break;
                
            case "CALL":                //<label>
                if(regex_label($words[1]) == TRUE)
                {
                    instruction_begin($instcruction_line, $words[0]);
                    print_label($words[1], 1);
                    instruction_end();
                    break;
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "RETURN":
                instruction_begin($instcruction_line, $words[0]);
                instruction_end();
            break;
                
            case "PUSHS":               //<symb>
                if(regex_symb($words[1]) == TRUE)
                {
                    instruction_begin($instcruction_line, $words[0]);
                    print_symb($words[1], 1);
                    instruction_end();
                    break;
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "POPS":                //<var>
                if(regex_var($words[1]) == TRUE)
                {
                    instruction_begin($instcruction_line, $words[0]);
                    print_var($words[1], 1);
                    instruction_end();
                    break;
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "ADD":                 //<var> <symb1> <symb2>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        if(regex_symb($words[3]) == TRUE)
                        {
                            instruction_begin($instcruction_line, $words[0]);
                            print_var($words[1], 1);
                            print_symb($words[2], 2);
                            print_symb($words[3], 3);
                            instruction_end();
                            break;
                        }
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "SUB":                 //<var> <symb1> <symb2>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        if(regex_symb($words[3]) == TRUE)
                        {
                            instruction_begin($instcruction_line, $words[0]);
                            print_var($words[1], 1);
                            print_symb($words[2], 2);
                            print_symb($words[3], 3);
                            instruction_end();
                            break;
                        }
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "MUL":                 //<var> <symb1> <symb2>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        if(regex_symb($words[3]) == TRUE)
                        {
                            instruction_begin($instcruction_line, $words[0]);
                            print_var($words[1], 1);
                            print_symb($words[2], 2);
                            print_symb($words[3], 3);
                            instruction_end();
                            break;
                        }
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "IDIV":                //<var> <symb1> <symb2>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        if(regex_symb($words[3]) == TRUE)
                        {
                            instruction_begin($instcruction_line, $words[0]);
                            print_var($words[1], 1);
                            print_symb($words[2], 2);
                            print_symb($words[3], 3);
                            instruction_end();
                            break;
                        }
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "LT":                  //<var> <symb1> <symb2>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        if(regex_symb($words[3]) == TRUE)
                        {
                            instruction_begin($instcruction_line, $words[0]);
                            print_var($words[1], 1);
                            print_symb($words[2], 2);
                            print_symb($words[3], 3);
                            instruction_end();
                            break;
                        }
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "GT":                  //<var> <symb1> <symb2>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        if(regex_symb($words[3]) == TRUE)
                        {
                            instruction_begin($instcruction_line, $words[0]);
                            print_var($words[1], 1);
                            print_symb($words[2], 2);
                            print_symb($words[3], 3);
                            instruction_end();
                            break;
                        }
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "EQ":                  //<var> <symb1> <symb2>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        if(regex_symb($words[3]) == TRUE)
                        {
                            instruction_begin($instcruction_line, $words[0]);
                            print_var($words[1], 1);
                            print_symb($words[2], 2);
                            print_symb($words[3], 3);
                            instruction_end();
                            break;
                        }
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "OR":                  //<var> <symb1> <symb2>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        if(regex_symb($words[3]) == TRUE)
                        {
                            instruction_begin($instcruction_line, $words[0]);
                            print_var($words[1], 1);
                            print_symb($words[2], 2);
                            print_symb($words[3], 3);
                            instruction_end();
                            break;
                        }
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "AND":                 //<var> <symb1> <symb2>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        if(regex_symb($words[3]) == TRUE)
                        {
                            instruction_begin($instcruction_line, $words[0]);
                            print_var($words[1], 1);
                            print_symb($words[2], 2);
                            print_symb($words[3], 3);
                            instruction_end();
                            break;
                        }
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "NOT":                 //<var> <symb1> <symb2>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        if(regex_symb($words[3]) == TRUE)
                        {
                            instruction_begin($instcruction_line, $words[0]);
                            print_var($words[1], 1);
                            print_symb($words[2], 2);
                            print_symb($words[3], 3);
                            instruction_end();
                            break;
                        }
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "INT2CHAR":            //<var> <symb>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        instruction_begin($instcruction_line, $words[0]);
                        print_var($words[1], 1);
                        print_symb($words[2], 2);
                        instruction_end();
                        break;
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "STRI2INT":            //<var> <symb1> <symb2>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        if(regex_symb($words[3]) == TRUE)
                        {
                            instruction_begin($instcruction_line, $words[0]);
                            print_var($words[1], 1);
                            print_symb($words[2], 2);
                            print_symb($words[3], 3);
                            instruction_end();
                            break;
                        }
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "READ":                //<var> <type>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_type($words[2]) == TRUE)
                    {
                        instruction_begin($instcruction_line, $words[0]);
                        print_var($words[1], 1);
                        print_type($words[2], 2);
                        instruction_end();
                        break;
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "WRITE":               //<symb>
                if(regex_symb($words[1]) == TRUE)
                {
                    instruction_begin($instcruction_line, $words[0]);
                    print_symb($words[1], 1);
                    instruction_end();
                    break;
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "CONCAT":              //<var> <symb1> <symb2>
                if(regex_symb($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        instruction_begin($instcruction_line, $words[0]);
                        print_symb($words[1], 1);
                        print_symb($words[2], 2);
                        instruction_end();
                        break;
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "STRLEN":              //<var> <symb>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        instruction_begin($instcruction_line, $words[0]);
                        print_var($words[1], 1);
                        print_symb($words[2], 2);
                        instruction_end();
                        break;
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "GETCHAR":             //<var> <symb1> <symb2>
                if(regex_symb($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        instruction_begin($instcruction_line, $words[0]);
                        print_symb($words[1], 1);
                        print_symb($words[2], 2);
                        instruction_end();
                        break;
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "SETCHAR":             //<var> <symb1> <symb2>
                if(regex_symb($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        instruction_begin($instcruction_line, $words[0]);
                        print_symb($words[1], 1);
                        print_symb($words[2], 2);
                        instruction_end();
                        break;
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "TYPE":                //<var> <symb>
                if(regex_var($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        instruction_begin($instcruction_line, $words[0]);
                        print_var($words[1], 1);
                        print_symb($words[2], 2);
                        instruction_end();
                        break;
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "LABEL":               //<label>
                if(regex_label($words[1]) == TRUE)
                {
                    instruction_begin($instcruction_line, $words[0]);
                    print_label($words[1], 1);
                    instruction_end();
                    break;
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "JUMP":                //<label>
                if(regex_label($words[1]) == TRUE)
                {
                    instruction_begin($instcruction_line, $words[0]);
                    print_label($words[1], 1);
                    instruction_end();
                    break;
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "JUMPIFEQ":            //<label> <symb1> <symb2>
                if(regex_symb($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        instruction_begin($instcruction_line, $words[0]);
                        print_symb($words[1], 1);
                        print_symb($words[2], 2);
                        instruction_end();
                        break;
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "JUMPIFNEQ":           //<label> <symb1> <symb2>
                if(regex_symb($words[1]) == TRUE)
                {
                    if(regex_symb($words[2]) == TRUE)
                    {
                        instruction_begin($instcruction_line, $words[0]);
                        print_symb($words[1], 1);
                        print_symb($words[2], 2);
                        instruction_end();
                        break;
                    }
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "EXIT":                //<symb>
                if(regex_symb($words[1]) == TRUE)
                {
                    instruction_begin($instcruction_line, $words[0]);
                    print_symb($words[1], 1);
                    instruction_end();
                    break;
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "DPRINT":              //<symb>
                if(regex_symb($words[1]) == TRUE)
                {
                    instruction_begin($instcruction_line, $words[0]);
                    print_symb($words[1], 1);
                    instruction_end();
                    break;
                }
                fwrite(STDERR, $error_lexikal_or_syntact);
                exit($error_lexikal_or_syntact);
            break;
                
            case "BREAK":               
                instruction_begin($instcruction_line, $words[0]);
                instruction_end();
            break;

            default:
                #v případě že nazazí na komentář, tak se dekrementuje počítadlo instrukcí
                if(preg_match('/.*#.*/', $words[0]) == TRUE)
                {
                    $instcruction_line--;
                }
                else
                {
                    fwrite(STDERR, $error_in_ippcode20);
                    exit($error_in_ippcode20 = 22);
                }
            break;
        }//zde končí SWITCH
    }//zde končí for pro řádky  
    echo "</program>\n";    //zde končí program
    fwrite(STDERR, $nonerror);
    exit($nonerror);
}

#toto když prvni řadek nesedi
else
{
    #případ že chybí hlavička tedy .IPPCODE20
    echo "Your program was beheaded. Be gone at once!!\n";
    fwrite(STDERR, $error_missing_head);
    exit($error_missing_head);
}

?>