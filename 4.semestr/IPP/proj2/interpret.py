# IPP proj2 - Interpret
# Autor: Václav Doleček

#pár slov na začátek:
# Nemám rád Python.

#To do list:
# dopsat návratové chybové hodnoty
# dělat defakto celý projekt
# smazat servisní část
# smířit se že bodový zisk bude minimální
# dopsat případ kdy je zadán mouze source resp čtení ze souboru
# nejspíše u WRITE nejsou všecky typy

# Jak s se seznamy:
# list [zde je index daného prvku][zde jsou atributy daného prvku]

import sys
import getopt
import argparse
from io import StringIO

import xml.etree.ElementTree as xml_tool

def main():

    # flags
    input_flag = 0
    source_flag = 0
    iter_flag = 0
    createframe_flag = 0
    # slouží pro zjištění v jakém rámci se má hledat
    frame_flag = 0

    # seznamy a jiné takové vytvory
    temp_file = []

    list_gf = [],[],[]
    list_gf_size = 0
    list_lf = [],[],[]
    list_lf_size = 0
    list_tf = [],[],[]
    list_tf_size = 0

    actual_place_read = 0 # použito k obsluze READ

    # country
    counter_run = 0


    # errory
    error_xml_missing = 31
    error_xml_unexpectet_structure = 32

    # list argumentu
    argumentList = sys.argv[1:] 
    
    # Options 
    options = "hmo:"
    
    # Long options 
    long_options = ["help", "source=", "input="] 
    
    try: 
        # Parsing argumentů 
        arguments, values = getopt.getopt(argumentList, options, long_options) 
        
        # kontrola každého argumentu 
        for currentArgument, currentValue in arguments: 
    
            if currentArgument in ("-h", "--help"): 
                help()
                
            elif currentArgument in ("", "--source"): 
                source_file =  currentValue
                source_flag = 1
                
            elif currentArgument in ("", "--input"): 
                input_file = currentValue
                input_flag = 1
    
    # řešení chyb
    except getopt.error as err: 
        print (str(err)) 

    if input_flag == 0 and source_flag == 0:
        return 11


    # pokud je zadán source jako file
    if source_flag == 1:
        # vytvoří se strom
        try:
            xml_tree = xml_tool.parse(source_file)
            xml_root = xml_tree.getroot()
            
        except xml_tool.ParseError as e:
            return error_xml_missing

    # pokud není source zadán jako file
    elif source_flag == 0:
        # našte se ze stdin
        iter_flag = 0
        while iter_flag == 0:
            try:
                line = input()
                if line:
                    temp_file.append(line)
                else:
                    break
            except EOFError as e:
                iter_flag = 1
            source_file = '\n'.join(temp_file)

            source_file = StringIO(source_file)
            
        # vytvoří se strom
        try:
            xml_tree = xml_tool.parse(source_file)
            xml_root = xml_tree.getroot()
        
        except xml_tool.ParseError as e:
            return error_xml_missing

    # pokud je input brán ze souboru
    if input_flag == 1:
        try:
            input_file_open = open(input_file, "r")
        except (OSError, IOError) as e:
            return 12

        if input_file_open.mode == 'r':
            input_file_content = input_file_open.read()
        else:
            return 12

        content_len = len(input_file_content)


    # pokud je input brán ze stdin
    elif input_flag == 0:
        
        while iter_flag == 0:
            try:
                line = input()
                if line:
                    temp_file.append(line)
                else:
                    break
            except EOFError as e:
                iter_flag = 1
            input_file = '\n'.join(temp_file)

            content_len = len(input_file)


    # prohledávání XML
    for instruct in xml_root:
        instruct_opcode = instruct.attrib.get('opcode')
        instruct_order = instruct.attrib.get('order')

        #příkazy samotné:
        # <var>
        if "DEFVAR" in instruct_opcode:
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                if "var" in var_type:
                    # pokud je to GF
                    if "GF" in var_name :
                        list_append(list_gf, list_gf_size, var_name)

                        list_append(list_gf, list_gf_size, var_type)

                        list_append(list_gf, list_gf_size, 0)
                        
                        list_gf_size = list_gf_size + 1
                    # pokud je to LF
                    elif "LF" in var_name: 
                        list_append(list_lf, list_lf_size, var_name)

                        list_append(list_lf, list_lf_size, var_type)

                        list_append(list_lf, list_lf_size, 0)

                        list_lf_size = list_lf_size + 1
                    # pokud je to TF
                    elif "TF" in var_name:
                        list_append(list_tf, list_tf_size, var_name)

                        list_append(list_tf, list_tf_size, var_type)

                        list_append(list_tf, list_tf_size, 0)

                        list_tf_size = list_tf_size + 1

                    else:
                        print("This frame does not exist")
                        return 55
                else:
                    print("DEFVAR - not oka")

        # <var> <symb>
        elif "MOVE" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    # des -> destination
                    # sour -> source
                    # nalezení cílové proměnné
                    if "GF" in var_name :
                        if counter_run == 0:
                            dest_index = list_search(list_gf_size, list_gf,   var_name)
                            frame_flag = 1
                        else:
                            sour_index = list_search(list_gf_size, list_gf, var_name)

                            var_name = list_gf[sour_index][2]

                    elif "LF" in var_name:
                        if counter_run == 0:
                            dest_index = list_search(list_lf_size, list_lf,   var_name)
                            frame_flag = 2
                        else:
                            sour_index = list_search(list_lf_size, list_lf,   var_name)

                            var_name = list_lf[sour_index][2]

                    elif "TF" in var_name and createframe_flag == 1:
                        if counter_run == 0:
                            dest_index = list_search(list_tf_size, list_tf,   var_name)
                            frame_flag = 3
                        else:
                            sour_index = list_search(list_tf_size, list_tf,   var_name)

                            var_name = list_tf[sour_index][2]

                    else:
                        print("Bad frame")
                        return 55

                else:
                    pass
                    if frame_flag == 1:
                        list_gf[dest_index][2] = var_name
                    elif frame_flag == 2:
                        list_lf[dest_index][2] = var_name
                    elif frame_flag == 3:
                        list_tf[dest_index][2] = var_name
                    else:
                        print("error")
            counter_run = counter_run + 1
             
        #
        elif "CREATEFRAME" in instruct_opcode:
            while list_tf_size != 0:
                i = 0
                while i != 2:
                    del list_tf[list_tf_size][i]
                    i = i + 1
            list_lf_size = 0
            createframe_flag = 1

        elif "PUSHFRAME" in instruct_opcode:
            print("PUSHFRAME")

        elif "POPFRAME" in instruct_opcode:
            print("POPFRAME")

        # <label>
        elif "CALL" in instruct_opcode:
            print("CALL")

        elif "RETURN" in instruct_opcode:
            print("RETURN")

        # <symb>
        elif "PUSHS" in instruct_opcode:
            print("PUSHS")

        # <var>
        elif "POP" in instruct_opcode:
            print("POP")

        # <var> <symb> <symb>
        elif "ADD" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                # pokud ano, zjistíme v jakém je rámci
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3

                    else:
                        print("Error")
                        return 53

                # v dalším kole se kontroluje, zda je další argument
                # typu int a pokud ano, zapamatujeme si ho
                elif "int" in var_type and counter_run == 1:
                    num_first = var_name

                # v posledním kole kontrolujeme, zda je i poslední
                # argument typu int a pokud ano, jsou 2. a 3. 
                # argumet sečten a vysledek je uložen do 1. argumentu
                elif "int" in var_type and counter_run == 2:
                    if frame_flag == 1:
                        list_gf[dest_index][2] = int(num_first) + int(var_name)
                    elif frame_flag == 2:
                        list_lf[dest_index][2] = int(num_first) + int(var_name)
                    elif frame_flag == 3:
                        list_tf[dest_index][2] = int(num_first) + int(var_name)
                    else:
                        print("ERROR")
                        return 54
                
                else:
                    print("error")
                    return 53
                
                counter_run = counter_run + 1


        # <var> <symb> <symb>
        elif "SUB" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                # pokud ano, zjistíme v jakém je rámci
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3

                    else:
                        print("Error")
                        return 53

                # v dalším kole se kontroluje, zda je další argument
                # typu int a pokud ano, zapamatujeme si ho
                elif "int" in var_type and counter_run == 1:
                    num_first = var_name

                # v posledním kole kontrolujeme, zda je i poslední
                # argument typu int a pokud ano, jsou 2. a 3. 
                # argumet sečten a vysledek je uložen do 1. argumentu
                elif "int" in var_type and counter_run == 2:
                    if frame_flag == 1:
                        list_gf[dest_index][2] = int(num_first) - int(var_name)
                    elif frame_flag == 2:
                        list_lf[dest_index][2] = int(num_first) - int(var_name)
                    elif frame_flag == 3:
                        list_tf[dest_index][2] = int(num_first) - int(var_name)
                    else:
                        print("ERROR")
                        return 54
                
                else:
                    print("error")
                    return 53
                
                counter_run = counter_run + 1

        # <var> <symb> <symb>
        elif "MUL" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                # pokud ano, zjistíme v jakém je rámci
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3

                    else:
                        print("Error")
                        return 53

                # v dalším kole se kontroluje, zda je další argument
                # typu int a pokud ano, zapamatujeme si ho
                elif "int" in var_type and counter_run == 1:
                    num_first = var_name

                # v posledním kole kontrolujeme, zda je i poslední
                # argument typu int a pokud ano, jsou 2. a 3. 
                # argumet sečten a vysledek je uložen do 1. argumentu
                elif "int" in var_type and counter_run == 2:
                    if frame_flag == 1:
                        list_gf[dest_index][2] = int(num_first) * int(var_name)
                    elif frame_flag == 2:
                        list_lf[dest_index][2] = int(num_first) * int(var_name)
                    elif frame_flag == 3:
                        list_tf[dest_index][2] = int(num_first) * int(var_name)
                    else:
                        print("ERROR")
                        return 54
                
                else:
                    print("error")
                    return 53
                
                counter_run = counter_run + 1

        # <var> <symb> <symb>
        elif "IDIV" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3

                    else:
                        print("Error")
                        return 53

                elif "int" in var_type and counter_run == 1:
                    num_first = var_name


                elif "int" in var_type and counter_run == 2:
                    try:
                        if frame_flag == 1:
                            list_gf[dest_index][2] = int(num_first) / int(var_name)
                        elif frame_flag == 2:
                            list_lf[dest_index][2] = int(num_first) / int(var_name)
                        elif frame_flag == 3:
                            list_tf[dest_index][2] = int(num_first) / int(var_name)
                        else:
                            return 54
                    except ZeroDivisionError:
                        return 57
                
                else:
                    print("error")
                    return 53
                
                counter_run = counter_run + 1

        # <var> <symb> <symb>
        elif "LT" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                # pokud ano, zjistíme v jakém je rámci
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3

                # v tomto kole si zapamatuje informace o 2. argumentu
                elif counter_run == 1:
                    first_smyb_type = var_type
                    first_smyb_value = var_name

                # nyní se provede samotná operace s 2. a 3. argumentem
                # a vysledek je uložen do 1. argumentu
                elif counter_run == 2:
                    if first_smyb_type == var_type:
                        if frame_flag == 1:
                            if first_smyb_value < var_name:
                                list_gf[dest_index][2] = "true"
                            else:
                                list_gf[dest_index][2] = "false"

                        if frame_flag == 2:
                            if first_smyb_value < var_name:
                                list_lf[dest_index][2] = "true"
                            else:
                                list_lf[dest_index][2] = "false"

                        if frame_flag == 3:
                            if first_smyb_value < var_name:
                                list_tf[dest_index][2] = "true"
                            else:
                                list_tf[dest_index][2] = "false"

                    else:
                        return 53   
                counter_run = counter_run + 1 


        # <var> <symb> <symb>
        elif "GT" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                # pokud ano, zjistíme v jakém je rámci
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3
                
                # v tomto kole si zapamatuje informace o 2. argumentu
                elif counter_run == 1:
                    first_smyb_type = var_type
                    first_smyb_value = var_name

                # nyní se provede samotná operace s 2. a 3. argumentem
                # a vysledek je uložen do 1. argumentu
                elif counter_run == 2:
                    if first_smyb_type == var_type:
                        if frame_flag == 1:
                            if first_smyb_value > var_name:
                                list_gf[dest_index][2] = "true"
                            else:
                                list_gf[dest_index][2] = "false"

                        if frame_flag == 2:
                            if first_smyb_value > var_name:
                                list_lf[dest_index][2] = "true"
                            else:
                                list_lf[dest_index][2] = "false"

                        if frame_flag == 3:
                            if first_smyb_value > var_name:
                                list_tf[dest_index][2] = "true"
                            else:
                                list_tf[dest_index][2] = "false"

                    else:
                        return 53   
                counter_run = counter_run + 1 

        # <var> <symb> <symb>
        elif "EQ" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                # pokud ano, zjistíme v jakém je rámci
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3
                
                # v tomto kole si zapamatuje informace o 2. argumentu
                elif counter_run == 1:
                    first_smyb_type = var_type
                    first_smyb_value = var_name

                # nyní se provede samotná operace s 2. a 3. argumentem
                # a vysledek je uložen do 1. argumentu
                elif counter_run == 2:
                    if first_smyb_type == var_type:
                        if frame_flag == 1:
                            if first_smyb_value == var_name:
                                list_gf[dest_index][2] = "true"
                            else:
                                list_gf[dest_index][2] = "false"

                        if frame_flag == 2:
                            if first_smyb_value == var_name:
                                list_lf[dest_index][2] = "true"
                            else:
                                list_lf[dest_index][2] = "false"

                        if frame_flag == 3:
                            if first_smyb_value == var_name:
                                list_tf[dest_index][2] = "true"
                            else:
                                list_tf[dest_index][2] = "false"

                    else:
                        return 53   
                counter_run = counter_run + 1 

        # <var> <symb> <symb>
        elif "AND" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                # pokud ano, zjistíme v jakém je rámci
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3
                
                # v tomto kole si zapamatuje informace o 2. argumentu
                elif "bool" in var_type and counter_run == 1:
                    first_smyb_value = var_name

                # nyní se provede samotná operace s 2. a 3. argumentem
                # a vysledek je uložen do 1. argumentu
                elif "bool" in var_type and counter_run == 2:
                        if frame_flag == 1:
                            if first_smyb_value == var_name and "true" in var_name:
                                list_gf[dest_index][2] = "true"
                            else:
                                list_gf[dest_index][2] = "false"

                        if frame_flag == 2:
                            if first_smyb_value == var_name and "true" in var_name:
                                list_lf[dest_index][2] = "true"
                            else:
                                list_lf[dest_index][2] = "false"

                        if frame_flag == 3:
                            if first_smyb_value == var_name and "true" in var_name:
                                list_tf[dest_index][2] = "true"
                            else:
                                list_tf[dest_index][2] = "false"

                else:
                    return 53   
                counter_run = counter_run + 1 

        # <var> <symb> <symb>
        elif "OR" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                # pokud ano, zjistíme v jakém je rámci
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3
                
                # v tomto kole si zapamatuje informace o 2. argumentu
                elif "bool" in var_type and counter_run == 1:
                    first_smyb_value = var_name

                # nyní se provede samotná operace s 2. a 3. argumentem
                # a vysledek je uložen do 1. argumentu
                elif "bool" in var_type and counter_run == 2:
                        if frame_flag == 1:
                            if first_smyb_value != "false" or var_name != "false":
                                list_gf[dest_index][2] = "true"
                            else:
                                list_gf[dest_index][2] = "false"

                        if frame_flag == 2:
                            if first_smyb_value != "false" or var_name != "false":
                                list_lf[dest_index][2] = "true"
                            else:
                                list_lf[dest_index][2] = "false"

                        if frame_flag == 3:
                            if first_smyb_value != "false" or var_name != "false":
                                list_tf[dest_index][2] = "true"
                            else:
                                list_tf[dest_index][2] = "false"

                else:
                    return 53   
                counter_run = counter_run + 1 

        # <var> <symb>
        elif "NOT" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                # pokud ano, zjistíme v jakém je rámci
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3
                
                # v tomto kole si zapamatuje informace o 2. argumentu
                elif "bool" in var_type and counter_run == 1:
                        if frame_flag == 1:
                            if "false" in var_name:
                                list_gf[dest_index][2] = "true"
                            else:
                                list_gf[dest_index][2] = "false"

                        if frame_flag == 2:
                            if "false" in var_name:
                                list_lf[dest_index][2] = "true"
                            else:
                                list_lf[dest_index][2] = "false"

                        if frame_flag == 3:
                            if "false" in var_name:
                                list_tf[dest_index][2] = "true"
                            else:
                                list_tf[dest_index][2] = "false"

                else:
                    return 53   
                counter_run = counter_run + 1

        # <var> <symb>
        elif "INT2CHAR" in instruct_opcode:
            print("INT2CHAR")

        # <var> <symb> <symv>
        elif "STRI2INT" in instruct_opcode:
            print("STRI2INT")

        # <var> <type>
        elif "READ" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                # pokud ano, zjistíme v jakém je rámci
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3
                
                # v tomto kole si zapamatuje informace o 2. argumentu
                elif "type" in var_type and counter_run == 1:
                    # rozsekání inputu na jednotlivé řádky pro input ze souboru
                    if input_flag == 1:
                        actual_word = ""
                        try:
                            while actual_place_read <= content_len:
                                if  input_file_content[actual_place_read] == "\n":
                                    actual_place_read = actual_place_read + 1
                                    break
                                else:
                                    actual_word = actual_word + input_file_content[actual_place_read]
                                    actual_place_read = actual_place_read + 1
                        except IndexError:
                            print("",end='')

                    # rozsekání inputu na jednotlivé řádky pro input ze stdin 
                    else:
                        actual_word = ""
                        try:
                            while actual_place_read <= content_len:
                                if  input_file[actual_place_read] == "\n":
                                    actual_place_read = actual_place_read + 1
                                    break
                                else:
                                    actual_word = actual_word + input_file[actual_place_read]
                                    actual_place_read = actual_place_read + 1
                        except IndexError:
                            print("",end='')

                    if frame_flag == 1:
                        list_gf[dest_index][1] = var_name
                        list_gf[dest_index][2] = actual_word
                       
                    if frame_flag == 2:
                        list_lf[dest_index][1] = var_name
                        list_lf[dest_index][2] = actual_word

                    if frame_flag == 3:
                        list_tf[dest_index][1] = var_name
                        list_tf[dest_index][2] = actual_word

                else:
                    return 53   
                counter_run = counter_run + 1
        # <symb>
        elif "WRITE" in instruct_opcode:
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                if "var" in var_type:
                    # pokud je to GF
                    if "GF" in var_name:
                        print_write(list_gf_size, list_gf, var_name)
                    
                    elif "LF" in var_name:
                        print_write(list_lf_size, list_lf, var_name)
                    
                    elif "TF" in var_name:
                        print_write(list_tf_size, list_tf, var_name)

                    else:
                        return 55
                   
                elif "int" in var_type:
                    print (var_name, end = '')
                
                elif "string" in var_type:
                    print (var_name, end = '')

                elif "bool" in var_type:
                    if "true" in var_name:
                        print("True", end = '')
                    elif "false" in var_name:
                        print("False", end = '')
                elif "nil" in var_type:
                    print("", end='') 
                else:
                    print("ERROR!!!!!!!!!!!!!!!!!!")
                    return 53

        # <var> <symb> <symb>
        elif "CONCAT" in instruct_opcode:
            print("CONCAT")

        # <var> <symb>
        elif "STRLEN" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                # pokud ano, zjistíme v jakém je rámci
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3
                
                # v tomto kole si zapamatuje informace o 2. argumentu
                elif counter_run == 1:
                    if frame_flag == 1:
                        list_gf[dest_index][1] = "int"
                        list_gf[dest_index][2] = len(var_name)

                    if frame_flag == 2:
                        list_lf[dest_index][1] = "int"
                        list_lf[dest_index][2] = len(var_name)

                    if frame_flag == 3:
                        list_tf[dest_index][1] = "int"
                        list_tf[dest_index][2] = len(var_name)



                counter_run = counter_run + 1

        # <var> <symb> <symb>
        elif "GETCHAR" in instruct_opcode:
            print("GETCHAR")

        # <var> <symb> <symb>
        elif "SETCHAR" in instruct_opcode:
            print("SETCHAR")

        # <var> <symb>
        elif "TYPE" in instruct_opcode:
            counter_run = 0
            #vytáhne to co nás zajímá z XML
            for args in instruct:
                var_type = args.attrib.get('type')
                var_name = args.text
                # kontrola zda se jedna o var
                if "var" in var_type and counter_run == 0:
                    # pokud je to GF
                    if "GF" in var_name:
                        dest_index = list_search(list_gf_size, list_gf,   var_name)
                        frame_flag = 1
                    
                    elif "LF" in var_name:
                        dest_index = list_search(list_lf_size, list_lf,   var_name)
                        frame_flag = 2

                    elif "TF" in var_name and createframe_flag == 1:
                        dest_index = list_search(list_tf_size, list_tf,   var_name)
                        frame_flag = 3

                elif counter_run == 1:
                    if frame_flag == 1:
                        list_gf[dest_index][2] = var_type
                    elif frame_flag == 2:
                        list_lf[dest_index][2] = var_type
                    elif frame_flag == 3:
                        list_tf[dest_index][2] = var_type
                else:
                    return 55
 
                counter_run = counter_run + 1 

        # <label>
        elif "LABEL" in instruct_opcode:
            print("LABEL")

        # <label>
        elif "JUMP" in instruct_opcode:
            print("JUMP")

        # <label> <symb> <symb>
        elif "JUMPIFEQ" in instruct_opcode:
            print("JUMPIFEQ")

        # <label> <symb> <symb>
        elif "JUMPIFNEQ" in instruct_opcode:
            print("JUMPIFNEQ")

        # <symb>
        elif "EXIT" in instruct_opcode:
            print("EXIT")

        # <symb>
        elif "DPRINT" in instruct_opcode:
            print("DPRINT")

        elif "BREAK" in instruct_opcode:
            print("BREAK")


# Funkce pro práci s pamětí

# funkce pro WRITE 
def print_write(size, list, name):
    wanted = list_search(size, list, name)

    if wanted == -1:
        return 54

    print(list[wanted][2], end = '')

# funkce pro vyhledávání indexu prvku
def list_search(size, list, looking_for):
    temp_place = 0
    while temp_place <= size:
        if looking_for in list[temp_place]:
            return temp_place
        else:
            temp_place = temp_place + 1
    return -1

# funkce která přidá prvek na požadované místo
def list_append(list, place, think):
    list[place].append(think)

#funkce pro vypsání help
def help():
    print("Hello, you choinse help option, hope this will help:")
    print("this is interpert for IPPcode20 writen in Python")
    print("Arguments:")
    print("\t--help -> you already know what is it for")
    print("\t--source=[file] -> source file for interpretion source code")
    print("\t--input=[file.xml] -> input file with valid XML code (IPPCODE20)")
    print("hope this will satisfy enought, if not...sorry I guess?")

    return 0




if __name__ == "__main__":
    main()
