from os import listdir
from os.path import isfile, join


DIRECTORY = "/home/kiki/task"


def find_output_file(directory):
    only_files = [f for f in listdir(directory) if isfile(join(directory, f))]

    for file_name in only_files:
        if file_name.startswith("result"):
            return join(directory, file_name)


def check_output_is_sorted(file_name: str) -> bool:
    f = open(file_name)
    new_line = None

    for line in f:
        previous_line = new_line
        new_line = line

        if previous_line is None:
            continue

        if previous_line > new_line:
            print(f"ERROR!!!"
                  f"{previous_line}  >>>> {new_line}")
            return False

    return True


def main():
    output_file_name = find_output_file(DIRECTORY)

    print(check_output_is_sorted(output_file_name))


main()
