from dataclasses import dataclass
from pathlib import Path

from data_frames import get_food_df


@dataclass
class FoodData: ...


def food_statistics(dir_path: Path) -> FoodData:
    """This function reads the food.csv file and returns the data in a FoodData object.

    Args:
        dir_path (Path): the path to the data folder.

    Returns:
        FoodData: a dataclass with the food data.
    """
    df = get_food_df(dir_path)
    ...
