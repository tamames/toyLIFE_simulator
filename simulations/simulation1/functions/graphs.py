"""In this file we define some python functions to study the data generated with the C++ code."""

from pathlib import Path

import matplotlib.pyplot as plt
import matplotlib.figure as mplf  # just for type hints
import pandas as pd

DIR_PATH = Path("simulations/simulation1/graphs")


def get_df(file: str) -> pd.DataFrame:
    """Reads a csv file and returns a pandas DataFrame with the data.
    we mainly have this types of files:
            food: information about the food pool.
                total: all the food for the generation
                stats: the stats of the food pool
            geno: information about the genotypes at each generation
            popu: information about the population at each generation
            total: all the info of one simulation in one file
    if the file doesn't match any of these cases, it will be read anyway.

    Args:
        file (str): The path to the file

    Returns:
        pd.DataFrame: the csv as a pandas DataFrame.
    """

    data_folder = Path("simulations/simulation1/data")
    file_to_open = data_folder / file
    print(file_to_open)

    if file.startswith("food"):
        type_of_food = file.split("_")[1]
        if type_of_food.startswith("stats"):
            types = {
                "Max_1s": int,
                "Index": int,
                "Binary": str,
                "Total 1s": int,
                "Total 0s": int,
                "Total Elements": int,
            }
            df = pd.read_csv(
                file_to_open, sep=",", header=0, dtype=types, index_col=False
            )
        elif type_of_food.startswith("total"):
            types = {"Binary": str}
            df = pd.read_csv(file_to_open, sep=",", header=0, dtype=types)
        else:
            print("Unknown type of food file. Reading anyway...")
            df = pd.read_csv(file_to_open, sep=",", header=0, index_col=False)

    elif file.startswith("geno"):
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

    elif file.startswith("popu"):
        types = {
            "Generation": int,
            "Average Energy Gain": float,
            "Max Energy Gain": int,
            "Min Energy Gain": int,
        }
        df = pd.read_csv(file_to_open, sep=",", header=0, dtype=types, index_col=False)

    elif file.startswith("total"):
        types = {
            "ID": int,
            "Genotype": str,
            "Energy": float,
            "Age": int,
            "Parent": int,
            "Iteration": int,
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
    file_to_save = str(DIR_PATH / name)
    fig.savefig(file_to_save)


def write_results(file_name: str, description: str, data_source: str = "") -> None:
    """Writes into the Readme to keep track of the figures created.

    Args:
        file_name (str): the name of the picture
        description (str): a description to identify the picture.
    """
    # First, we open the description file to write what is going to be in the file.
    readme_path = DIR_PATH / "Readme.md"

    with open(readme_path, "a") as des_file:
        des_file.write(file_name + ".png" + " &rarr " + description + "  \n")


def save_and_write(
    fig: mplf.Figure, data_source: str, plot_type: str, description: str = ""
) -> None:
    """Handle the call of the save_fig and write_results functions.

    Args:
        file (mplf.Figure): the figure to save
        name (str): The name of the file to save
        description (str): a description to identify the picture.
    """

    fig_name = get_fig_name(data_source, plot_type)

    # in this case we don't need to check if we are going to overwrite the image because
    # we can always recreate a missing image with the data source.
    save_graph(fig, fig_name)
    params_for_descrp = fig_name.split("_")

    if not description:
        description = f"Is a {params_for_descrp[0]} plot that represent a {params_for_descrp[1]}. *Data = {data_source}*. **Control={params_for_descrp[-1]}**"

    write_results(fig_name, description, data_source)


# TODO improve the representation functions
def bar_only_1(df: pd.DataFrame) -> mplf.Figure:
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


def stackplot_1_0(df: pd.DataFrame) -> tuple[mplf.Figure, str]:
    plot_type = "stack"
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
        colors=["#157F1F", "#07020D"],
        alpha=1.0,
    )

    # Customize the plot
    ax.set_xlabel("Generation")
    ax.set_ylabel("Ratio")
    ax.set_title("Distribution of 1s and 0s in Population")
    ax.legend()
    ax.grid(axis="y", linestyle="--", alpha=0.7)

    return fig, plot_type


def food_histogram(df: pd.DataFrame) -> tuple[mplf.Figure, str]:
    """This generates a histogram to see the distribution of a list of genotypes.

    Args:
        df (pd.DataFrame): The data frame with the binary strings

    Returns:
        tuple[mplf.Figure, str]: the plot and the type of plot
    """
    plot_type = "hist"
    size = len(df["Binary"][0])
    # Count the number of 0s and 1s in each binary string
    df["1s Count"] = df["Binary"].apply(lambda x: x.count("1"))
    df["0s Count"] = size - df["1s Count"]

    # Create a histogram for 0s count
    fig, ax = plt.subplots(figsize=(10, 6))

    # Create a histogram for the count of 1s
    bins = range(size + 1)  # Create bins from 0 to size
    ax.hist(df["1s Count"], bins=bins, alpha=0.7, color="b", label="1s Count")

    # Customize the plot
    ax.set_xlabel("Count of 1s")
    ax.set_ylabel("Frequency")
    ax.set_title("Distribution of 1s in Binary Strings")
    ax.legend()
    ax.grid(axis="y", linestyle="--", alpha=0.7)

    return fig, plot_type


def get_fig_name(source_name: str, plot_type: str) -> str:
    """Gives the figure name based on the file name.
    The figures name have the following structure:
        plotType_source_control
    so with the file name we can get what type of plot we are looking at, what is representing
    (this is just if it comes from a food, geno or popu file) and the control parameter of that simulation,
    if we want to know the data source we can look at the Readme file.

    Args:
        source_name (str): the data source
        plot_type (str): the type of plot

    Returns:
        str: the name of the figure file
    """
    control = source_name.split(".")[0][-2:]
    return f"{plot_type}_{source_name[:4]}_{control}"


def main() -> None:
    file_name = "genotype_info_10_25.csv"
    # the last two characters of the file name are the control parameter of the population
    #  that number gives us the way the foodList is distributed.

    df = get_df(file_name)
    fig1, plot_type = stackplot_1_0(df)
    # print(df.head())

    save_and_write(fig1, file_name, plot_type)


def do_all(file_name: str) -> None:
    """This function does everything given a file name
    The idea is that given a file name the program generates all the valuable plots for that type
    of data.

    Args:
        file_name (str): the data source
    """
    pass


if __name__ == "__main__":
    main()
