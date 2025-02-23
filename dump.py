import os

# Define output files
STRUCTURE_FILE = "project_structure.txt"
DUMP_FILE = "project_dump.txt"
EXCLUDED_DIRS = {"build", ".git"}  # Directories to exclude
INCLUDED_EXTENSIONS = {".cpp", ".h", "CMakeLists.txt", ".md", ".bat"}  # File types to include

def generate_project_structure():
    """Generates a tree structure of the project excluding specified directories."""
    with open(STRUCTURE_FILE, "w", encoding="utf-8") as struct_file:
        for root, dirs, files in os.walk("."):
            # Remove excluded directories from the list of dirs to avoid walking them
            dirs[:] = [d for d in dirs if d not in EXCLUDED_DIRS]
            
            # Print and write the directory name
            level = root.count(os.sep)
            indent = "    " * level
            struct_file.write(f"{indent}{os.path.basename(root)}/\n")
            
            # Print and write the file names
            for file in files:
                struct_file.write(f"{indent}    {file}\n")

def generate_project_dump():
    """Dumps the content of selected files into one file with file labels."""
    with open(DUMP_FILE, "w", encoding="utf-8") as dump_file:
        for root, dirs, files in os.walk("."):
            # Remove excluded directories from the list of dirs to avoid walking them
            dirs[:] = [d for d in dirs if d not in EXCLUDED_DIRS]

            for file in files:
                file_ext = os.path.splitext(file)[1]
                
                if file in INCLUDED_EXTENSIONS or file_ext in INCLUDED_EXTENSIONS:
                    file_path = os.path.join(root, file)
                    try:
                        with open(file_path, "r", encoding="utf-8") as f:
                            dump_file.write(f"\n=== FILE: {file_path} ===\n")
                            dump_file.write(f.read())
                            dump_file.write("\n")
                    except Exception as e:
                        dump_file.write(f"\n=== FILE: {file_path} (Error reading file: {e}) ===\n")

if __name__ == "__main__":
    print("Generating project structure...")
    generate_project_structure()
    print(f"Saved: {STRUCTURE_FILE}")

    print("Generating project dump...")
    generate_project_dump()
    print(f"Saved: {DUMP_FILE}")

    print("Process complete!")
