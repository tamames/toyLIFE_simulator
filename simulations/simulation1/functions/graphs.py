"""In this file we define some python function to graph the data generated with the C++ code."""

from pathlib import Path

import matplotlib.pyplot as plt
import matplotlib.figure as mplf  # just for type hints
import pandas as pd


def get_df(file: str) -> pd.DataFrame:
    """Reads a csv file and returns a pandas DataFrame with the data.
    we mainly have three types of files:
            food: information about the food pool
            geno: information about the genotypes at each generation
            popu: information about the population at each generation
    if the file doesn't match any of these cases, it will be read anyway.

    Args:
        file (str): The path to the file

    Returns:
        pd.DataFrame: the csv as a pandas DataFrame.
    """
    type_of_file = file[:4]

    data_folder = Path("simulations/simulation1/data")
    file_to_open = data_folder / file

    if type_of_file == "food":
        types = {
            "Max_1s": int,
            "Index": int,
            "Binary": str,
            "Total 1s": int,
            "Total 0s": int,
            "Total Elements": int,
        }
        df = pd.read_csv(file_to_open, sep=",", header=0, dtype=types, index_col=False)

    elif type_of_file == "geno":
        types = {
            "Max_1s": int,
            "Index": int,
            "Binary": str,
            "Total 1s": int,
            "Total 0s": int,
            "Total Elements": int,
            "Generation": int,
        }
        df = pd.read_csv(file_to_open, sep=",", header=0, dtype=types, index_col=False)

    elif type_of_file == "popu":
        types = {
            "Generation": int,
            "Average Energy Gain": float,
            "Max Energy Gain": int,
            "Min Energy Gain": int,
        }
        df = pd.read_csv(file_to_open, sep=",", header=0, dtype=types, index_col=False)
    else:
        print(f"The file {file} doesn't match any case. Reading anyway...")
        df = pd.read_csv(file_to_open, sep=",", header=0, index_col=False)

    return df


def save_graph(fig: mplf.Figure, name: str) -> None:
    """Saves a graph in the folder graphs.

    Args:
        file (mplf.Figure): the figure to save
        name (str): The name of the file to save
    """

    file_to_save = "simulations/simulation1/graphs/" + name

    fig.savefig(file_to_save)


def write_results(file_name: str, description: str) -> None:
    """Writes into the Readme to keep track of the figures created.

    Args:
        file_name (str): the name of the picture
        description (str): a description to identify the picture.
    """
    # First, we open the description file to write what is going to be in the file.
    readme_path = Path("simulations/simulation1/graphs/Readme.md")

    with open(readme_path, "a") as des_file:
        des_file.write(file_name + ".png" + " → " + description + "  \n")


def graph_distribution_of_1(df: pd.DataFrame) -> mplf.Figure:
    df["1s Ratio"] = df["Total 1s"] / df["Total Elements"]

    # Create a new figure
    fig, ax = plt.subplots(figsize=(10, 6))

    # Create the bar plot
    ax.bar(df["Generation"], df["1s Ratio"], color="b", alpha=0.7)
    ax.set_xlabel("Generation")
    ax.set_ylabel("Ratio of 1s")
    ax.set_title("Distribution of 1s in Population")
    ax.grid(axis="y", linestyle="--", alpha=0.7)

    return fig


def graph_1s_and_0s(df: pd.DataFrame) -> mplf.Figure:
    df["1s Ratio"] = df["Total 1s"] / df["Total Elements"]

    df["0s Ratio"] = 1 - df["1s Ratio"]

    # Create a new figure
    fig, ax = plt.subplots(figsize=(10, 6))

    # Create the stacked area plot
    ax.stackplot(
        df["Generation"],
        df["1s Ratio"],
        df["0s Ratio"],
        labels=["1s Ratio", "0s Ratio"],
        colors=["blue", "red"],
        alpha=0.7,
    )

    # Customize the plot
    ax.set_xlabel("Generation")
    ax.set_ylabel("Ratio")
    ax.set_title("Distribution of 1s and 0s in Population")
    ax.legend()
    ax.grid(axis="y", linestyle="--", alpha=0.7)

    return fig


def main() -> None:
    file_name = "genotype_info_75.csv"
    # the last two characters of the file name is the control parameter of the population, that number gives us the way the foodList is distributed.
    control = file_name.split(".")[0][-2:]
    df = get_df(file_name)
    fig1 = graph_1s_and_0s(df)
    save_graph(fig1, "1s_and_0s.png")


if __name__ == "__main__":
    main()
