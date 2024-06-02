import logging
from dataclasses import dataclass
from pathlib import Path
from warnings import simplefilter


from ForGraphs.data_frames import get_food_df
import ForGraphs.plots as fpl
import pandas as pd
from matplotlib.pyplot import close

simplefilter(action="ignore", category=pd.errors.SettingWithCopyWarning)


@dataclass(kw_only=True)
class _FoodData:
    amount_of_food: int
    food_len: int
    number_of_1: int
    mean_of_1: float
    percentage_of_1: float


def _write_food_data(
    data_folder_path: Path, food_data: _FoodData, value_counts_mode: pd.DataFrame
) -> None:
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

        readme.write("\n## Food Modes  \n")
        readme.write(value_counts_mode.to_markdown(index=False))
        readme.write("\n\n")

    logging.info("Finish writing food data to Readme.md")


def _food_statistics(data_folder_path: Path) -> _FoodData:
    """This function reads the food.csv file and returns the data in a _FoodData object.
    Only for the first iteration, that is, the original food.

    Args:
        dir_path (Path): the path to the data folder.

    Returns:
        _FoodData: a dataclass with the food data.
    """
    df = get_food_df(data_folder_path, 1)
    logging.info("Start calculating food statistics")
    df_0 = df[df["Iteration"] == 0]
    food_len = len(df_0.iloc[0]["Binary"])

    amount_of_food = len(df_0)

    total_slots = food_len * amount_of_food

    df_0["1_count"] = df_0["Binary"].apply(lambda x: x.count("1"))
    df_0["0_count"] = food_len - df_0["1_count"]

    number_of_1 = df_0["1_count"].sum()
    mean_of_1 = df_0["1_count"].mean()

    percentage_of_1 = number_of_1 / total_slots

    logging.info("Finish calculating food statistics")

    return _FoodData(
        amount_of_food=amount_of_food,
        food_len=food_len,
        number_of_1=number_of_1,
        mean_of_1=mean_of_1,
        percentage_of_1=percentage_of_1,
    )


def _transform_df(data_folder_path: Path) -> pd.DataFrame:

    logging.info("Start loading binaries from food.csv")
    by_iteration = get_food_df(data_folder_path, 3)
    logging.info("Finish loading binaries from food.csv")

    logging.debug("Calculating ratios")
    by_iteration["total_slots"] = by_iteration["1_count"] + by_iteration["0_count"]
    by_iteration["1s_ratio"] = by_iteration["1_count"] / by_iteration["total_slots"]
    by_iteration["0s_ratio"] = 1 - by_iteration["1s_ratio"]
    logging.debug("Finish calculating ratios")

    return by_iteration


def main(data_folder_path: Path) -> None:
    logging.info("Start treating food.csv")

    food_object = _food_statistics(data_folder_path)
    value_counts_modes = get_food_df(data_folder_path, 2)
    _write_food_data(data_folder_path, food_object, value_counts_modes)

    rest_iteration = get_food_df(data_folder_path, 5)

    fig = fpl.stackplot_several_types(rest_iteration)
    fpl.save_fig(fig, fpl.PlotType.FOOD_TYPE, data_folder_path / "graphs")
    close(fig)

    for_stack_plot = _transform_df(data_folder_path)
    size_per_iteration = get_food_df(data_folder_path, 4)

    fig = fpl.stackplot_1_0(for_stack_plot, total=False)
    fpl.save_fig(fig, fpl.PlotType.FOOD_STACK, data_folder_path / "graphs")
    close(fig)

    fig = fpl.food_size(size_per_iteration)
    fpl.save_fig(fig, fpl.PlotType.FOOD_SIZE, data_folder_path / "graphs")
    close(fig)
