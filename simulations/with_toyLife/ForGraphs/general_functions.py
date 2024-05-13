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


def get_energy_to_reproduce(data_folder_path: Path) -> str:
    """Reads the Readme.md inside the data_folder_path and extracts the
    energy to reproduce parameter.

    Args:
        data_folder_path (Path): the data folder we are looking at.

    Returns:
        str: the energy to reproduce
    """
    check_file_exists(data_folder_path / "Readme.md")

    logging.info("Start reading the Readme.md to find the energy reproduce parameter")
    with open(data_folder_path / "Readme.md", "r") as readme:
        for line in readme:
            if "Energy to reproduce" in line:
                logging.info("Finish reading the Readme.md")
                return line.split(" ")[-1].strip()

    raise ValueError(
        f"The energy to reproduce parameter wasn't found in {data_folder_path=}"
    )
