"""In this file we define some python functions to study the data generated with the C++ code."""

import logging
import os
import shutil
from pathlib import Path
from typing import Generator

import ForGraphs.config as config
import ForGraphs.food as food
import ForGraphs.plots as plots
import ForGraphs.total as total

#### FUNCTIONS ####


def check_folders() -> Generator[Path, None, None]:
    """Iterates through the simulations folder and check if the graphs folder exists.
    If it doesn't the function yield the path to the folder.

    Returns:
        Path: the path object to the folder of simulation that we have to process.
    """
    for folder in os.listdir(config.DATA_PATH):
        graph_path = Path(config.DATA_PATH / folder / "graphs")
        if not graph_path.exists():
            # this folder hasn't been processed yet
            yield Path(config.DATA_PATH / folder)


def create_graph_folder(data_folder_path: Path) -> None:
    """Creates the graphs folder inside the data_folder_path.

    Args:
        data_folder_path (Path): the path to the folder where we are going to create the graphs folder.
    """
    graph_folder = data_folder_path / "graphs"
    os.mkdir(graph_folder)


def del_graphs_folder(*args: int) -> None:
    """Deletes the graphs folder inside each simulation_i folder.
    In case args is empty it deletes all the graphs folders.
    Otherwise it deletes the graphs folder of the simulations_i in args.
    """
    if args:
        for i in args:
            folder_path = Path(config.DATA_PATH / f"simulation_{i}" / "graphs")
            shutil.rmtree(folder_path, ignore_errors=True)
    else:
        for folder in os.listdir(config.DATA_PATH):
            folder_path = Path(config.DATA_PATH / folder / "graphs")
            shutil.rmtree(folder_path, ignore_errors=True)


#### MAIN FUNCTION ####


def main() -> None:

    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s.%(msecs)03d -> %(message)s",
        datefmt="%H:%M:%S",
        handlers=[
            logging.FileHandler("simulations\\with_toyLife\\python_log.log"),
            logging.StreamHandler(),
        ],
    )

    for folder_to_process in check_folders():
        logging.info(f"Start processing folder {folder_to_process}")
        create_graph_folder(folder_to_process)
        food.main(folder_to_process)
        total.main(folder_to_process)
        plots.main(folder_to_process)
        logging.info(f"Finish processing folder {folder_to_process}")


if __name__ == "__main__":
    main()
