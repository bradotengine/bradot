import os

def rename_files(directory):
    for root, dirs, files in os.walk(directory):
        for file_name in files:
            new_name = file_name
            
            # Substituições de acordo com as instruções
            #new_name = new_name.replace("Godot", "Bradot")
            #new_name = new_name.replace("godot", "bradot")
            #new_name = new_name.replace(".gd", ".br")
            #new_name = new_name.replace("gdscript", "brscript")
            #new_name = new_name.replace("gdextension", "brextension")
            #new_name = new_name.replace("GDExtension", "BRExtension")
            new_name = new_name.replace("GD0", "BR0")
            
            # Renomeia o arquivo se o nome foi alterado
            if new_name != file_name:
                old_path = os.path.join(root, file_name)
                new_path = os.path.join(root, new_name)
                os.rename(old_path, new_path)
                print(f'Renamed: {file_name} -> {new_name}')

# Caminho do diretório que você quer renomear
directory_path = '.'

rename_files(directory_path)

