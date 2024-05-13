"""In this file we put the functions related with getting the data frames for statistics and graphs"""

from pathlib import Path

import pandas as pd

try:
    from ForGraphs.general_functions import check_file_exists
except ModuleNotFoundError:
    from general_functions import check_file_exists


def get_total_df(
    data_folder_path: Path, usecols: None | list[str] = None
) -> pd.DataFrame:
    """Returns the total DataFrame from the data_folder_path.

    Args:
        data_folder_path (Path): the path to the data folder we are looking at.
        usecols (None | list[str], optional): the columns we want to use. Defaults to None, so we get all columns.

    Returns:
        pd.DataFrame: a pandas DataFrame with the total data.
    """
    check_file_exists(data_folder_path / "total.csv")

    return pd.read_csv(
        data_folder_path / "total.csv",
        sep=";",
        header=0,
        index_col=False,
        usecols=usecols,
        dtype={
            "ID": int,
            "Genotype": str,
            "Energy": float,
            "Age": int,
            "Parent": int,
            "Prots": str,
            "Mets": str,
            "Dims": str,
            "Iteration": int,
        },
    )


def get_energy_df(data_folder_path: Path) -> pd.DataFrame:
    """Returns the energies DataFrame from the data_folder_path.

    Args:
        data_folder_path (Path): the path to the data folder we are looking at.

    Returns:
        pd.DataFrame: a pandas DataFrame with the energies data.
    """
    check_file_exists(data_folder_path / "energies.csv")

    return pd.read_csv(
        data_folder_path / "energies.csv",
        sep=",",
        header=0,
        index_col=False,
        dtype={"Max": float, "Average": float, "Min": float},
    )


def get_energy_gains_df(data_folder_path: Path) -> pd.DataFrame:
    """Returns the energy gain DataFrame from the data_folder_path.

    Args:
        data_folder_path (Path): the path to the data folder we are looking at.

    Returns:
        pd.DataFrame: a pandas DataFrame with the energy gain data.
    """
    check_file_exists(data_folder_path / "energy_gains.csv")

    return pd.read_csv(
        data_folder_path / "energy_gains.csv",
        sep=",",
        header=0,
        index_col=False,
        dtype={
            "Max_Gain": float,
            "Average_Gain": float,
            "Total_Gain": float,
            "Min_Gain": float,
        },
    )


def get_sizes_df(data_folder_path: Path) -> pd.DataFrame:
    """Returns the energies DataFrame from the data_folder_path.

    Args:
        data_folder_path (Path): the path to the data folder we are looking at.

    Returns:
        pd.DataFrame: a pandas DataFrame with the sizes data.
    """
    check_file_exists(data_folder_path / "sizes.csv")

    return pd.read_csv(
        data_folder_path / "sizes.csv",
        sep=",",
        header=0,
        index_col=False,
        dtype={"Size": int},
    )


def get_food_df(data_folder_path: Path) -> pd.DataFrame:
    """Returns the food DataFrame from the data_folder_path.

    Args:
        data_folder_path (Path): the path to the data folder we are looking at.

    Returns:
        pd.DataFrame: a pandas DataFrame with the food data.
    """
    check_file_exists(data_folder_path / "food.csv")

    return pd.read_csv(
        data_folder_path / "food.csv",
        sep=",",
        header=0,
        index_col=False,
        dtype={"Iteration": int, "Binary": str},
    )


def get_dead_reproduce_df(data_folder_path: Path) -> pd.DataFrame:
    """Returns the dead_reproduce table that tells us how much agents
    died and reproduce in each iteration.

    Args:
        data_folder_path (Path): the path to the data folder we are looking at.

    Returns:
        pd.DataFrame: a pandas DataFrame with the dead_reproduce data.
    """
    check_file_exists(data_folder_path / "deads_reproduce.csv")

    return pd.read_csv(
        data_folder_path / "deads_reproduce.csv",
        sep=",",
        header=0,
        index_col=False,
        dtype={"Deads": int, "Reproduce": int},
    )
