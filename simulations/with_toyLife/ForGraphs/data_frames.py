"""In this file we put the functions related with getting the data frames for statistics and graphs"""

from pathlib import Path

import pandas as pd
from ForGraphs.general_functions import check_file_exists


def get_total_df(data_path: Path) -> pd.DataFrame:
    """Returns the total DataFrame from the data_path.

    Args:
        data_path (Path): the path to the data folder we are looking at.

    Returns:
        pd.DataFrame: a pandas DataFrame with the total data.
    """
    check_file_exists(data_path / "total.csv")

    return pd.read_csv(
        data_path / "total.csv",
        sep=";",
        header=0,
        index_col=False,
        dtype={
            "ID": int,
            "Genotype": str,
            "Energy": float,
            "Age": int,
            "Parent": int,
            "Prots": str,
            "Mets": str,
            "Dims": str,
            "Iteration": str,
        },
    )


def get_energy_df(data_path: Path) -> pd.DataFrame:
    """Returns the energies DataFrame from the data_path.

    Args:
        data_path (Path): the path to the data folder we are looking at.

    Returns:
        pd.DataFrame: a pandas DataFrame with the energies data.
    """
    check_file_exists(data_path / "energies.csv")

    return pd.read_csv(
        data_path / "energies.csv",
        sep=",",
        header=0,
        index_col=False,
        dtype={"Max": float, "Average": float, "Min": float},
    )


def get_sizes_df(data_path: Path) -> pd.DataFrame:
    """Returns the energies DataFrame from the data_path.

    Args:
        data_path (Path): the path to the data folder we are looking at.

    Returns:
        pd.DataFrame: a pandas DataFrame with the sizes data.
    """
    check_file_exists(data_path / "sizes.csv")

    return pd.read_csv(
        data_path / "sizes.csv",
        sep=",",
        header=0,
        index_col=False,
        dtype={"Size": float},
    )


def get_food_df(data_path: Path) -> pd.DataFrame:
    """Returns the food DataFrame from the data_path.

    Args:
        data_path (Path): the path to the data folder we are looking at.

    Returns:
        pd.DataFrame: a pandas DataFrame with the food data.
    """
    check_file_exists(data_path / "food_total.csv")

    return pd.read_csv(
        data_path / "food_total.csv",
        sep=",",
        header=0,
        index_col=False,
        dtype={"Binary": str},
    )
