from pathlib import Path
import operations as op


def check_file_exists(file_path: Path) -> None:
    if not file_path.exists():
        raise FileNotFoundError(
            f"The file {file_path.name} doesn't exists in {file_path.parent}."
        )


def get_energy_to_reproduce(data_path: Path) -> str:
    """Reads the Readme.md inside the folder_path and extracts the
    energy to reproduce parameter.

    Args:
        data_path (Path): the data folder we are looking at.

    Returns:
        str: the energy to reproduce
    """
    check_file_exists(data_path / "Readme.md")

    with open(data_path / "Readme.md", "r") as readme:
        for line in readme:
            if "Energy to reproduce" in line:
                return line.split(" ")[-1].strip()

    raise ValueError(f"The energy to reproduce parameter wasn't found in {data_path=}")


def write_food_data(data_path: Path, food_data: op.FoodData) -> None:
    """Writes the food data into the Readme.md file.

    Args:
        data_path (Path): the data folder we are looking at.
        food (int): the food parameter.
    """

    with open(data_path / "Readme.md", "a") as readme:
        readme.write("---\r\n")
        readme.write("## Food Data\r\n")
