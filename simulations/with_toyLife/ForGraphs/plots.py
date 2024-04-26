from pathlib import Path
from enum import Enum

import ForGraphs.data_frames as fdf
import matplotlib.figure as mplf  # just for type hints
import matplotlib.pyplot as plt
from ForGraphs.config import FIG_SIZE
from ForGraphs.general_functions import get_energy_to_reproduce


class PlotType(Enum):
    """This enum is used to define the different types of plots that can be created."""

    ENERGY = "energy"
    SIZES = "sizes"


def energy_plot(data_folder_path: Path, plot_reproduce: bool = False) -> mplf.Figure:
    """This plot correspond to the max, min and average energy of the population at each generation.

    Args:
        data_folder_path (Path): the data path where the data is stored.
        plot_reproduce (bool, optional): whether to plot the energy to reproduce or not. Defaults to False.

    Returns:
        mplf.Figure: the plot with the energy data.
    """
    fig, ax = plt.subplots(figsize=FIG_SIZE)

    if plot_reproduce:
        energy_to_reproduce = get_energy_to_reproduce(data_folder_path)
        ax.axhline(
            y=int(energy_to_reproduce),
            color="k",
            linestyle="--",
            label="Energy to Reproduce",
        )

    df = fdf.get_energy_df(data_folder_path)

    # Create the line plot
    ax.plot(df["Max"], color="r", label="Max Energy")
    ax.plot(df["Average"], color="g", label="Average Energy")
    ax.plot(df["Min"], color="b", label="Min Energy")

    ax.set_xlabel("Generation")
    ax.set_ylabel("Energy")
    ax.set_title("Energy of the population across each generation.")
    ax.legend()
    return fig


def sizes_plot(data_folder_path: Path) -> mplf.Figure:
    """This function creates a simple line plot with the size of the population at each generation.

    Args:
        data_folder_path (Path): It's only one column with the size of each generation.

    Returns:
        mplf.Figure:
    """

    df = fdf.get_sizes_df(data_folder_path)

    fig, ax = plt.subplots(figsize=FIG_SIZE)

    # Create the line plot
    ax.plot(df["Size"], color="b", label="Population Size")

    ax.set_xlabel("Generation")
    ax.set_ylabel("Size")
    ax.set_title("Size of the Population")
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
    fig_name = (
        f"{plot_type.value}_{extra_name}.pdf"
        if extra_name
        else f"{plot_type.value}.pdf"
    )
    fig_path = graph_folder / fig_name
    fig.savefig(str(fig_path), format="pdf", bbox_inches="tight")


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

    sizes_fig = sizes_plot(data_folder_path)
    save_fig(sizes_fig, PlotType.SIZES, graph_folder)
    plt.close(energy_fig)
