from pathlib import Path

import data_frames as fdf
import matplotlib.figure as mplf  # just for type hints
import matplotlib.pyplot as plt
from config import FIG_SIZE
from general_functions import get_energy_to_reproduce


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

    df = fdf.get_energy_df(data_path)

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

    df = fdf.get_sizes_df(data_path)

    fig, ax = plt.subplots(figsize=FIG_SIZE)

    # Create the line plot
    ax.plot(df["Size"], color="b", label="Population Size")

    ax.set_xlabel("Generation")
    ax.set_ylabel("Size")
    ax.set_title("Size of the Population")
    return fig
