"""In this file we put the functions related with getting the data frames for statistics and graphs"""

import pandas as pd
from pathlib import Path

from general_functions import check_file_exists


def get_total_df(data_path: Path) -> pd.DataFrame:
    """Returns the total DataFrame from the data_path.

    Args:
        data_path (Path): the path to the data folder we are looking at.

    Raises:
        FileNotFoundError: in case the total file doesn't exists.

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

    Raises:
        FileNotFoundError: in case the energies file doesn't exists.

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

    Raises:
        FileNotFoundError: in case the sizes file doesn't exists.

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
