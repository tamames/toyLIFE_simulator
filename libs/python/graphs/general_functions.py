import logging
from pathlib import Path


def check_file_exists(file_path: Path) -> None:
    """Just check if a file exists.

    Args:
        file_path (Path): the path to the file.

    Raises:
        FileNotFoundError: in case the file doesn't exists.
    """
    if not file_path.exists():
        raise FileNotFoundError(
            f"The file {file_path.name} doesn't exists in {file_path.parent}."
        )


def get_energy_to_reproduce(data_folder_path: Path) -> tuple[str, str, str, str]:
    """Reads the Readme.md inside the data_folder_path and extracts the
    energy to reproduce parameter.

    Args:
        data_folder_path (Path): the data folder we are looking at.

    Returns:
        str: the energy to reproduce.
    """
    check_file_exists(data_folder_path / "Readme.md")

    logging.info("Start reading the Readme.md to find the energy reproduce parameter")
    with open(data_folder_path / "Readme.md", "r") as readme:
        for line in readme:
            if "Energy to reproduce" in line:
                logging.info("Energy to reproduce parameter found")
                energy_reproduce = line.split(" ")[-1].strip()

            elif "Energy to die" in line:
                logging.info("Finish reading the Readme.md")
                energy_die = line.split(" ")[-1].strip()

            elif "Translation energy" in line:
                translation_energy = line.split(" ")[-1].strip()

            elif "Breaking energy" in line:
                breaking_energy = line.split(" ")[-1].strip()
                return energy_reproduce, energy_die, translation_energy, breaking_energy

    raise ValueError(
        f"The energy to reproduce parameter wasn't found in {data_folder_path=}"
    )


def get_initial_food(data_folder_path: Path) -> str:
    """Reads the Readme.md inside the data_folder_path and extracts the
    original food_size parameter.

    Args:
        data_folder_path (Path): the data folder we are looking at.

    Returns:
        str: the original food size.
    """
    check_file_exists(data_folder_path / "Readme.md")

    logging.info("Start reading the Readme.md to find the original food size parameter")
    with open(data_folder_path / "Readme.md", "r") as readme:
        for line in readme:
            if "Original Food Size" in line:
                logging.info("Finish reading the Readme.md")
                return line.split(" ")[-1].strip()

    raise ValueError(
        f"The energy to reproduce parameter wasn't found in {data_folder_path=}"
    )
