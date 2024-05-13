import logging
from dataclasses import dataclass
from pathlib import Path

from ForGraphs.data_frames import get_food_df
import ForGraphs.plots as fpl
import pandas as pd
from matplotlib.pyplot import close


@dataclass(kw_only=True)
class _FoodData:
    amount_of_food: int
    food_len: int
    number_of_1: int
    mean_of_1: float
    percentage_of_1: float


def _write_food_data(data_folder_path: Path, food_data: _FoodData) -> None:
    """Writes the food data into the Readme.md file.

    Args:
        data_folder_path (Path): the data folder we are looking at.
        food (int): the food parameter.
    """

    logging.info("Start writing food data to Readme.md")
    with open(data_folder_path / "Readme.md", "a") as readme:
        readme.write("\n## Food Data  \n")
        for key, value in food_data.__dict__.items():
            if key == "percentage_of_1":
                readme.write(f"{key} = {value:.2%}  \n")
            else:
                readme.write(f"{key} = {value}  \n")

    logging.info("Finish writing food data to Readme.md")


def _food_statistics(df: pd.DataFrame) -> _FoodData:
    """This function reads the food.csv file and returns the data in a _FoodData object.
    Only for the first iteration, that is, the original food.

    Args:
        dir_path (Path): the path to the data folder.

    Returns:
        _FoodData: a dataclass with the food data.
    """

    logging.info("Start calculating food statistics")
    food_len = len(df.iloc[0]["Binary"])
    amount_of_food = len(df)

    total_slots = food_len * amount_of_food

    df["1_count"] = df["Binary"].apply(lambda x: x.count("1"))
    df["0_count"] = food_len - df["1_count"]

    number_of_1 = df["1_count"].sum()
    mean_of_1 = df["1_count"].mean()

    percentage_of_1 = number_of_1 / total_slots
    logging.info("Finish calculating food statistics")

    return _FoodData(
        amount_of_food=amount_of_food,
        food_len=food_len,
        number_of_1=number_of_1,
        mean_of_1=mean_of_1,
        percentage_of_1=percentage_of_1,
    )


def _transform_df(df: pd.DataFrame) -> tuple[pd.DataFrame, pd.Series]:

    logging.debug("Start counting ones in food")
    df["1_count"] = df["Binary"].apply(lambda x: x.count("1"))
    logging.debug("Finish counting ones in food")

    df["1_count"] = df["1_count"].astype(int)

    genotype_size = len(df["Binary"].iloc[0])

    logging.debug("Subtracting 1s from total slots")
    df["0_count"] = genotype_size - df["1_count"]
    logging.debug("Finish subtracting 1s from total slots")

    logging.debug("Grouping by iteration")
    by_iteration = df.groupby(by=["Iteration"], as_index=False).agg(
        {"1_count": "sum", "0_count": "sum"}
    )
    logging.debug("Finish grouping by iteration")

    logging.debug("Calculating ratios")
    by_iteration["total_slots"] = by_iteration["1_count"] + by_iteration["0_count"]
    by_iteration["1s_ratio"] = by_iteration["1_count"] / by_iteration["total_slots"]
    by_iteration["0s_ratio"] = 1 - by_iteration["1s_ratio"]
    logging.debug("Finish calculating ratios")

    size_per_iteration = df["Iteration"].value_counts().sort_index()

    return by_iteration, size_per_iteration


def main(data_folder_path: Path) -> None:
    logging.info("Start loading food.csv")
    try:
        df = get_food_df(data_folder_path)
        logging.info("Finish loading food.csv")
    except FileNotFoundError:
        logging.warning("food.csv wasn't found")
        return
    food_object = _food_statistics(df[df["Iteration"] == 0])
    _write_food_data(data_folder_path, food_object)

    for_stack_plot, size_per_iteration = _transform_df(df)

    fig = fpl.stackplot_1_0(for_stack_plot, total=False)
    fpl.save_fig(fig, fpl.PlotType.FOOD_STACK, data_folder_path / "graphs")
    close(fig)

    fig = fpl.food_size(size_per_iteration)
    fpl.save_fig(fig, fpl.PlotType.FOOD_SIZE, data_folder_path / "graphs")
    close(fig)
