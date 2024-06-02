"""In this file we put the functions related with getting the data frames for statistics and graphs"""

from pathlib import Path
import logging

import pandas as pd

try:
    from ForGraphs.general_functions import check_file_exists, get_initial_food
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


def get_food_df(data_folder_path: Path, mode: int) -> pd.DataFrame:
    """Because the food DataFrame can be quite large we have different modes to read it.
    mode 0: read the whole DataFrame.
    mode 1: read only the original food.
    mode 2: read the value counts of the modes.
    mode 3: read the iterations to check the 1 and 0 count.
    mode 4: read the iterations to check the pool size.
    mode 5: read the iterations to check the modes.

    Args:
        data_folder_path (Path): the path to the data folder we are looking at.
        mode (int): the mode we want to use to read the data.

    Returns:
        pd.DataFrame: a pandas DataFrame with the specific food data.
    """
    check_file_exists(data_folder_path / "food.csv")
    if mode == 0:
        # total data
        logging.info("Reading the whole food.csv file")
        return pd.read_csv(
            data_folder_path / "food.csv",
            sep=",",
            header=0,
            index_col=False,
            dtype={"Iteration": int, "Binary": str, "Mode": "category"},
        )

    elif mode == 1:
        # only the original food
        logging.info("Reading only the original food")
        number_rows = int(get_initial_food(data_folder_path))
        return pd.read_csv(
            data_folder_path / "food.csv",
            sep=",",
            header=0,
            index_col=False,
            nrows=number_rows,
            usecols=["Iteration", "Binary"],
            dtype={"Iteration": int, "Binary": str},
        )

    elif mode == 2:
        logging.info("Reading the food modes")
        # get the value counts of the modes
        modes = {"O": 0, "P": 0, "N": 0, "R": 0}

        for chunk in pd.read_csv(
            data_folder_path / "food.csv",
            sep=",",
            header=0,
            index_col=False,
            chunksize=10000,
            dtype={"Mode": "category"},
            usecols=["Mode"],
        ):
            values = chunk["Mode"].value_counts()
            for key, value in values.items():
                modes[key] += value

        return pd.DataFrame(list(modes.items()), columns=["Mode", "count"]).sort_values(
            "count", ascending=False
        )

    elif mode == 3:
        logging.info("Reading the food iterations to check the 1 and 0 count.")
        chunk_results = []
        for chunk in pd.read_csv(
            data_folder_path / "food.csv",
            sep=",",
            header=0,
            index_col=False,
            chunksize=10000,
            dtype={"Iteration": int, "Binary": str},
            usecols=["Iteration", "Binary"],
        ):
            chunk["1_count"] = chunk["Binary"].apply(lambda x: x.count("1"))
            chunk["1_count"] = chunk["1_count"].astype(int)

            genotype_size = len(chunk["Binary"].iloc[0])

            chunk["0_count"] = genotype_size - chunk["1_count"]

            by_iteration = chunk.groupby(by=["Iteration"], as_index=False).agg(
                {"1_count": "sum", "0_count": "sum"}
            )

            chunk_results.append(by_iteration)

        return pd.concat(chunk_results).groupby("Iteration", as_index=False).sum()

    elif mode == 4:
        # for size of pool per iteration
        logging.info("Reading the food iterations to check the pool size.")
        sizes = {}

        for chunk in pd.read_csv(
            data_folder_path / "food.csv",
            sep=",",
            header=0,
            index_col=False,
            chunksize=10000,
            dtype={"Iteration": int},
            usecols=["Iteration"],
        ):
            values = chunk["Iteration"].value_counts()
            for key, value in values.items():
                if key in sizes:
                    sizes[key] += value
                else:
                    sizes[key] = value

        return (
            pd.DataFrame(list(sizes.items()), columns=["Iteration", "count"])
            .set_index("Iteration")
            .sort_index()
        )

    elif mode == 5:
        logging.info("Reading the food iterations to check the modes.")
        mode_counts = {}

        for chunk in pd.read_csv(
            data_folder_path / "food.csv",
            sep=",",
            header=0,
            index_col=False,
            chunksize=10000,
            dtype={"Iteration": int, "Mode": "category"},
            usecols=["Iteration", "Mode"],
        ):

            grouped_chunk = (
                chunk.groupby(["Iteration", "Mode"]).size().reset_index(name="count")
            )

            for _, row in grouped_chunk.iterrows():
                iteration = row["Iteration"]
                mode_type = row["Mode"]
                count = row["count"]
                if iteration not in mode_counts:
                    mode_counts[iteration] = {"O": 0, "N": 0, "P": 0, "R": 0}
                mode_counts[iteration][mode_type] += count

        final_data = []
        for iteration, counts in mode_counts.items():
            row = {
                "Iteration": iteration,
                "O": counts["O"],
                "N": counts["N"],
                "P": counts["P"],
                "R": counts["R"],
            }
            final_data.append(row)

        modes_df = pd.DataFrame(final_data)

        # Set the iteration as the index
        modes_df.set_index("Iteration", inplace=True)

        logging.debug("Finished counting modes per iteration")

        return modes_df

    else:
        raise ValueError("The mode has to be between 0 and 5")


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
