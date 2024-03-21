"""In this file we define some python functions to study the data generated with the C++ code."""

from pathlib import Path
import os

import matplotlib.pyplot as plt
import matplotlib.figure as mplf  # just for type hints
import pandas as pd

DIR_PATH = Path("simulations/with_toyLife/graphs")
FIG_SIZE = (10, 6)


def get_df(file: str) -> pd.DataFrame:
    """Reads a csv file and returns a pandas DataFrame with the data.
    we mainly have this types of files:
            energy: the max, mean and min energy of each generation
            sizes: size of the population at each generation
    if the file doesn't match any of these cases, it will be read anyway.

    Args:
        file (str): The path to the file

    Returns:
        pd.DataFrame: the csv as a pandas DataFrame.
    """

    data_folder = Path("simulations/with_toyLife/data")
    file_to_open = data_folder / file
    print(file_to_open)

    if file.startswith("energies"):
        types = {
            "Max": float,
            "Average": float,
            "Min": float,
        }
        df = pd.read_csv(file_to_open, sep=",", header=0, dtype=types, index_col=False)

    elif file.startswith("sizes"):
        types = {
            "Size": float,
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


def write_results(file_name: str, description: str) -> None:
    """Writes into the Readme to keep track of the figures created.

    Args:
        file_name (str): the name of the picture
        description (str): a description to identify the picture.
    """
    # First, we open the description file to write what is going to be in the file.
    readme_path = DIR_PATH / "Readme.md"

    with open(readme_path, "a") as des_file:
        des_file.write(file_name + ".png" + " &rarr " + description + "  \n")


def save_and_write(fig: mplf.Figure, name: str, description: str = "") -> None:
    """Handle the call of the save_fig and write_results functions.

    Args:
        file (mplf.Figure): the figure to save
        name (str): The name of the file to save
        description (str): a description to identify the picture.
    """

    # in this case we don't need to check if we are going to overwrite the image because
    # we can always recreate a missing image with the data source.
    save_graph(fig, name)

    # if not description:
    #     params_for_descrp = name.split("_")
    #     description = f"Is a {params_for_descrp[0]} plot that represent a {params_for_descrp[1]}. *Data = {data_source}*. **Control={params_for_descrp[-1]}**"

    write_results(name, description)


# TODO improve the representation functions
def bar_only_1(df: pd.DataFrame) -> mplf.Figure:
    df["1s Ratio"] = df["Total 1s"] / df["Total Elements"]

    # Create a new figure
    fig, ax = plt.subplots(figsize=FIG_SIZE)

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
    fig, ax = plt.subplots(figsize=FIG_SIZE)

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
    fig, ax = plt.subplots(figsize=FIG_SIZE)

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


def energy_plot(data_path: Path, plot_reproduce: bool = False) -> mplf.Figure:
    """This plot correspond to the max, min and average energy of the population at each generation.

    Args:
        data_path (Path): the data path where the data is stored.
        plot_reproduce (bool, optional): whether to plot the energy to reproduce or not. Defaults to False.

    Returns:
        mplf.Figure: the plot with the energy data.
    """
    fig, ax = plt.subplots(figsize=FIG_SIZE)

    if plot_reproduce:
        energy_to_reproduce = get_energy_to_reproduce(data_path)
        ax.axhline(
            y=int(energy_to_reproduce),
            color="k",
            linestyle="--",
            label="Energy to Reproduce",
        )

    df = get_energy_df(data_path)

    # Create the line plot
    ax.plot(df["Max"], color="r", label="Max Energy")
    ax.plot(df["Average"], color="g", label="Average Energy")
    ax.plot(df["Min"], color="b", label="Min Energy")

    ax.set_xlabel("Generation")
    ax.set_ylabel("Energy")
    ax.set_title("Energy of the population across each generation.")
    ax.legend()
    return fig


def sizes_plot(data_path: Path) -> mplf.Figure:
    """This function creates a simple line plot with the size of the population at each generation.

    Args:
        data_path (Path): It's only one column with the size of each generation.

    Returns:
        mplf.Figure:
    """
    check_file_exists(data_path / "sizes.csv")

    df = get_sizes_df(data_path)

    fig, ax = plt.subplots(figsize=FIG_SIZE)

    # Create the line plot
    ax.plot(df["Size"], color="b", label="Population Size")

    ax.set_xlabel("Generation")
    ax.set_ylabel("Size")
    ax.set_title("Size of the Population")
    return fig


#### UTILS FUNCTIONS ####


def check_file_exists(file_path: Path) -> None:
    if not file_path.exists():
        raise FileNotFoundError(
            f"The file {file_path.name} doesn't exists in {file_path.parent}."
        )


def check_for_plots():
    """This function checks every simulation_ folder to see if we have
    already create the graphs for the data of that simulation."""

    data_folder = Path("simulations\\with_toyLife\\data")
    for sim_folder in os.listdir(data_folder):

        sim_folder_path = data_folder / sim_folder
        graph_folder_path = data_folder / sim_folder / "graphs"

        if not graph_folder_path.is_dir():
            yield sim_folder_path


def get_energy_to_reproduce(data_path: Path) -> str:
    """Reads the Readme.md inside the folder_path and extracts the
    energy to reproduce parameter.

    Args:
        data_path (Path): the data folder we are looking at.

    Returns:
        str: the energy to reproduce
    """
    check_file_exists(data_path / "Readme.md")

    with open(data_path / "Readme.md", "r") as readme:
        for line in readme:
            if "Energy to reproduce" in line:
                return line.split(" ")[-1].strip()

    raise ValueError(f"The energy to reproduce parameter wasn't found in {data_path=}")


#### DF FUNCTIONS ####


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


#### MAIN FUNCTION ####


def main() -> None:
    pass


if __name__ == "__main__":
    main()
