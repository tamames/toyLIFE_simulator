from dataclasses import dataclass
from pathlib import Path

from ForGraphs.data_frames import get_food_df


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

    with open(data_folder_path / "Readme.md", "a") as readme:
        readme.write("---  \n")
        readme.write("## Food Data  \n")
        for key, value in food_data.__dict__.items():
            if key == "percentage_of_1":
                readme.write(f"{key} = {value:.1%}  \n")
            else:
                readme.write(f"{key} = {value}  \n")


def _food_statistics(data_folder_path: Path) -> _FoodData:
    """This function reads the food.csv file and returns the data in a _FoodData object.

    Args:
        dir_path (Path): the path to the data folder.

    Returns:
        _FoodData: a dataclass with the food data.
    """
    df = get_food_df(data_folder_path)
    food_len = len(df.iloc[0]["Binary"])
    amount_of_food = len(df)

    total_slots = food_len * amount_of_food

    df["1_count"] = df["Binary"].apply(lambda x: x.count("1"))
    df["0_count"] = food_len - df["1_count"]

    number_of_1 = df["1_count"].sum()
    mean_of_1 = df["1_count"].mean()

    percentage_of_1 = number_of_1 / total_slots

    return _FoodData(
        amount_of_food=amount_of_food,
        food_len=food_len,
        number_of_1=number_of_1,
        mean_of_1=mean_of_1,
        percentage_of_1=percentage_of_1,
    )


def main(data_folder_path: Path) -> None:
    try:
        food_object = _food_statistics(data_folder_path)
    except FileNotFoundError:
        # this simulation didn't have a food.csv file yet
        return
    _write_food_data(data_folder_path, food_object)
