import logging
import logging.config
from enum import Enum
from pathlib import Path

import matplotlib.figure as mplf  # just for type hints
import matplotlib.pyplot as plt
import pandas as pd

logging.config.dictConfig(
    {
        "version": 1,
        "disable_existing_loggers": True,
    }
)

logging.getLogger("matplotlib").setLevel(logging.WARNING)


try:
    import ForGraphs.data_frames as fdf
    from ForGraphs.config import FIG_SIZE
    from ForGraphs.general_functions import get_energy_to_reproduce
except ModuleNotFoundError:
    import data_frames as fdf
    from config import FIG_SIZE
    from general_functions import get_energy_to_reproduce


class PlotType(Enum):
    """This enum is used to define the different types of plots that can be created."""

    ENERGY = "energy"
    GAIN = "energy_gain"
    SIZES = "sizes"
    TOTAL = "total"
    DEAD = "dead"
    REPRODUCE = "reproduce"
    FOOD_STACK = "food_stack"
    FOOD_SIZE = "food_size"


def energy_plot(data_folder_path: Path, plot_reproduce: bool = False) -> mplf.Figure:
    """This plot correspond to the max, min and average energy of the population at each generation.

    Args:
        data_folder_path (Path): the data path where the data is stored.
        plot_reproduce (bool, optional): whether to plot the energy to reproduce or not. Defaults to False.

    Returns:
        mplf.Figure: the plot with the energy data.
    """

    logging.info("Start loading the energy data")
    df = fdf.get_energy_df(data_folder_path)
    logging.info("Finish loading the energy data")

    # Create the line plot
    logging.info("Start creating the energy plot")

    fig, ax = plt.subplots(figsize=FIG_SIZE)

    if plot_reproduce:
        energy_to_reproduce = get_energy_to_reproduce(data_folder_path)
        ax.axhline(
            y=int(energy_to_reproduce),
            color="k",
            linestyle="--",
            label="Energy to Reproduce",
        )

    ax.plot(df["Max"], color="r", label="Max Energy")
    ax.plot(df["Average"], color="g", label="Average Energy")
    ax.plot(df["Min"], color="b", label="Min Energy")

    ax.set_xlabel("Generation")
    ax.set_ylabel("Energy")
    ax.set_title("Energy of the population across each generation.")
    ax.legend()
    logging.info("Finish creating the energy plot")
    return fig


def energy_gain_plot(data_folder_path: Path) -> mplf.Figure:
    """This plot correspond to the max, min, total and average energy gains of the population at each generation.

    Args:
        data_folder_path (Path): the data path where the data is stored.

    Returns:
        mplf.Figure: the plot with the energy data.
    """

    logging.info("Start loading the energy gains data")
    df = fdf.get_energy_gains_df(data_folder_path)
    logging.info("Finish loading the energy gains data")

    # Create the line plot
    fig, ax = plt.subplots(figsize=FIG_SIZE)

    logging.info("Start creating the energy gains plot")
    ax.plot(df["Max_Gain"], color="r", label="Max Gain")
    ax.plot(df["Average_Gain"], color="y", label="Average Gain")
    ax.plot(df["Total_Gain"], color="g", label="Total Gain")
    ax.plot(df["Min_Gain"], color="b", label="Min Gain")

    ax.set_xlabel("Generation")
    ax.set_ylabel("Energy Gain")
    ax.set_title("Energy gain of the population across each generation.")
    ax.legend()
    logging.info("Finish creating the energy gains plot")
    return fig


def sizes_plot(data_folder_path: Path) -> mplf.Figure:
    """This function creates a simple line plot with the size of the population at each generation.

    Args:
        data_folder_path (Path): It's only one column with the size of each generation.

    Returns:
        mplf.Figure:
    """
    logging.info("Start loading the sizes data")
    df = fdf.get_sizes_df(data_folder_path)
    logging.info("Finish loading the sizes data")

    logging.info("Start creating the sizes plot")
    fig, ax = plt.subplots(figsize=FIG_SIZE)

    # Create the line plot
    ax.plot(df["Size"], color="b", label="Population Size")

    ax.set_xlabel("Generation")
    ax.set_ylabel("Size")
    ax.set_title("Size of the Population")
    logging.info("Finish creating the sizes plot")
    return fig


def stackplot_1_0(by_iteration: pd.DataFrame, total: bool) -> mplf.Figure:

    logging.info("Start creating the stacked area plot")
    # Create a new figure
    fig, ax = plt.subplots(figsize=FIG_SIZE)

    # Create the stacked area plot
    ax.stackplot(
        by_iteration["Iteration"],
        by_iteration["1s_ratio"],
        by_iteration["0s_ratio"],
        labels=["1s Ratio", "0s Ratio"],
        colors=["#F9C80E", "#F86624"],
        alpha=1.0,
    )

    # Customize the plot
    x_label = "Generation" if total else "Iteration"
    ax.set_xlabel(x_label)
    ax.set_ylabel("Ratio")
    title = (
        "Distribution of 1s and 0s in Population"
        if total
        else "Distribution of 1s and 0s in Food"
    )
    ax.set_title(title)
    ax.legend()
    ax.grid(axis="y", linestyle="--", alpha=0.7)
    logging.info("Finish creating the stacked area plot")

    return fig


def dead_reproduce_plot(data_folder_path: Path) -> tuple[mplf.Figure, mplf.Figure]:
    """This function creates two line plots, one with the number of agents that died in each generation
    and the other with the number of agents that reproduced in each generation.

    Args:
        data_folder_path (Path): the folder where the data is stored.

    Returns:
        tuple[mplf.Figure, mplf.Figure]: the two figures with the plots. First the dead agents and then the reproduced agents.
    """

    logging.info("Start loading the dead and reproduce data")
    df = fdf.get_dead_reproduce_df(data_folder_path)
    logging.info("Finish loading the dead and reproduce data")

    logging.info("Start creating the dead plot")
    fig1, ax1 = plt.subplots(figsize=FIG_SIZE)
    # Create the line plot
    ax1.plot(df["Deads"], color="#002FA7", label="Deceased Agents")

    ax1.set_xlabel("Generation")
    ax1.set_ylabel("Number of Agents")
    ax1.set_title("Agents that died in each Generation")
    logging.info("Finish creating the dead plot")

    logging.info("Start creating the reproduce plot")
    fig2, ax2 = plt.subplots(figsize=FIG_SIZE)
    ax2.plot(df["Reproduce"], color="#002FA7", label="Reproduced Agents")

    ax2.set_xlabel("Generation")
    ax2.set_ylabel("Number of Agents")
    ax2.set_title("Agents that reproduced in each Generation")
    logging.info("Finish creating the reproduce plot")

    return fig1, fig2


def food_size(sizes: pd.Series) -> mplf.Figure:
    """This function creates a simple line plot with the size of the food pool after each iteration.

    Args:
        sizes (pd.Series[int]): the info obtained from the _transform_df function in food.py.

    Returns:
        mplf.Figure: the figure with the food sizes plot.
    """

    logging.info("Start creating the food sizes plot")
    fig, ax = plt.subplots(figsize=FIG_SIZE)

    # Create the line plot
    ax.plot(sizes, color="b", label="Food Size")

    ax.set_xlabel("Generation")
    ax.set_ylabel("Size")
    ax.set_title("Size of Food pool after each iteration")
    logging.info("Finish creating the food sizes plot")
    return fig


def save_fig(
    fig: mplf.Figure, plot_type: PlotType, graph_folder: Path, extra_name: str = ""
) -> None:
    """This function saves the figure in the corresponding folder.

    Args:
        fig (mplf.Figure): the figure to save.
        plot_type (PlotType): the type of plot we are saving.
        graph_folder (Path): the path to the data folder.
    """

    logging.info(f"Saving the {plot_type.value} plot")
    fig_name = (
        f"{plot_type.value}_{extra_name}.pdf"
        if extra_name
        else f"{plot_type.value}.pdf"
    )
    fig_path = graph_folder / fig_name
    fig.savefig(str(fig_path), format="pdf", bbox_inches="tight")
    logging.info(f"Finish saving the {plot_type.value} plot")


def main(data_folder_path: Path) -> None:
    """This is the main function that generates the plots for the data in the data_folder_path.

    Args:
        data_folder_path (Path): the path to the data folder.
    """
    graph_folder = data_folder_path / "graphs"

    energy_fig = energy_plot(data_folder_path)
    save_fig(energy_fig, PlotType.ENERGY, graph_folder)
    plt.close(energy_fig)
    energy_fig = energy_plot(data_folder_path, plot_reproduce=True)
    save_fig(energy_fig, PlotType.ENERGY, graph_folder, extra_name="reproduce")
    plt.close(energy_fig)

    energy_gain_fig = energy_gain_plot(data_folder_path)
    save_fig(energy_gain_fig, PlotType.GAIN, graph_folder)
    plt.close(energy_gain_fig)

    sizes_fig = sizes_plot(data_folder_path)
    save_fig(sizes_fig, PlotType.SIZES, graph_folder)
    plt.close(energy_fig)

    dead_fig, reproduce_fig = dead_reproduce_plot(data_folder_path)
    save_fig(dead_fig, PlotType.DEAD, graph_folder)
    plt.close(dead_fig)
    save_fig(reproduce_fig, PlotType.REPRODUCE, graph_folder)
    plt.close(reproduce_fig)
