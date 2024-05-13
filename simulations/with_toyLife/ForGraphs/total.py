import logging
from dataclasses import dataclass
from pathlib import Path

try:
    from ForGraphs.data_frames import get_total_df
    from ForGraphs.plots import stackplot_1_0, save_fig, PlotType
except ModuleNotFoundError:
    from data_frames import get_total_df
    from plots import stackplot_1_0, save_fig, PlotType

import pandas as pd
from matplotlib.pyplot import close


@dataclass
class _DataMolecules:
    maximum_molecules: int
    table_molecules: str
    number_unique_molecules: int


@dataclass(kw_only=True)
class _GenotypeData:
    """this dataclass is the same as _FoodData in food.py."""

    genotype_len: int
    number_of_1: int
    mean_of_1: float
    percentage_of_1: float


@dataclass(kw_only=True)
class _TotalData:
    maximun_age: int
    median_age: float
    maximum_iterations: int
    number_of_children: int
    number_of_total_individuals: int
    number_of_total_genotypes: int
    data_prots: _DataMolecules | None
    data_mets: _DataMolecules | None
    data_dims: _DataMolecules | None
    genotype_data: _GenotypeData


def _total_statistics(df: pd.DataFrame) -> _TotalData:

    logging.info("Start processing data")
    maximun_age = df["Age"].max()
    logging.debug("Start grouping by ID, Age max")
    median_age = df[["ID", "Age"]].groupby("ID").max().mean()[0]
    logging.debug("Finish grouping by ID, Age max")

    logging.debug("Start getting maximum iterations")
    maximum_iterations = df["Iteration"].max()
    logging.debug("Finish getting maximum iterations")

    logging.debug("Start getting uniques")
    number_of_children = df[df["Parent"] > 0]["Parent"].nunique()
    number_of_total_individuals = df["ID"].nunique()
    number_of_total_genotypes = df["Genotype"].nunique()
    logging.debug("Finish getting uniques")

    genotype_data = _process_genotype_data(df["Genotype"])

    data_prots = _process_molecules(df["Prots"])
    data_mets = _process_molecules(df["Mets"])
    data_dims = _process_molecules(df["Dims"])

    logging.info("Finish processing data")

    return _TotalData(
        maximun_age=maximun_age,
        median_age=median_age,
        maximum_iterations=maximum_iterations,
        number_of_children=number_of_children,
        number_of_total_individuals=number_of_total_individuals,
        number_of_total_genotypes=number_of_total_genotypes,
        data_prots=data_prots,
        data_mets=data_mets,
        data_dims=data_dims,
        genotype_data=genotype_data,
    )


def _process_genotype_data(genotype_column: pd.Series) -> _GenotypeData:
    """This function is almost a copy of _food_statistics in food.py.
    We do it this way to get the same data for food and genotypes so we can compare them.

    Args:
        genotype_column (pd.Series): the column with genotypes.

    Returns:
        _GenotypeData: a dataclass with the genotype data.
    """

    logging.info("Start processing genotypes")
    genotype_len = len(genotype_column[0])
    amount_of_food = len(genotype_column)

    total_slots = genotype_len * amount_of_food

    logging.debug("Counting 1s in genotypes")
    one_count: pd.Series = genotype_column.apply(lambda x: x.count("1"))
    logging.debug("Finish counting 1s in genotypes")

    logging.debug("Adding and averaging 1s in genotypes")
    number_of_1 = one_count.sum()
    mean_of_1 = one_count.mean()
    logging.debug("Finish adding and averaging 1s in genotypes")

    percentage_of_1 = number_of_1 / total_slots

    logging.info("Finish processing genotypes")

    return _GenotypeData(
        genotype_len=genotype_len,
        number_of_1=number_of_1,
        mean_of_1=mean_of_1,
        percentage_of_1=percentage_of_1,
    )


def _process_molecules(molecule_column: pd.Series) -> _DataMolecules | None:
    logging.info(f"Start processing {molecule_column.name}")
    not_empty = molecule_column.dropna()
    if not_empty.empty:
        logging.info(f"{molecule_column.name} is empty")
        return

    logging.debug("Replacing and splitting")
    not_empty = not_empty.apply(
        lambda x: x.replace(" ", "").replace("{", "").replace("}", "")
    )
    df = not_empty.str.split(",", expand=True)
    # now we have a df where each row is an agent.
    # in each column we have the molecule and the amount.
    logging.debug("Finish replacing and splitting")

    maximum_molecules = len(df.columns)

    logging.debug("Stacking")
    stacked = df.stack()
    logging.debug("Finish stacking")

    logging.debug("Splitting molecule and amount")
    total_molecules = stacked.str.split(":", expand=True)
    logging.debug("Finish splitting molecule and amount")
    total_molecules.columns = ["molecule", "amount"]

    total_molecules = total_molecules.astype(
        dtype={"molecule": "category", "amount": int}
    )

    logging.debug("Grouping by molecule")
    total_molecules = total_molecules.groupby("molecule").sum()
    logging.debug("Finish grouping by molecule")

    number_unique_molecules = total_molecules.shape[0]

    logging.debug("Sorting values and markdown")
    table_molecules = (
        total_molecules[total_molecules["amount"] > 1]
        .sort_values(by="amount", ascending=False)
        .head(75)
        .to_markdown()
    )
    logging.debug("Finish sorting values and markdown")

    logging.info(f"Finish processing {molecule_column.name}")
    return _DataMolecules(maximum_molecules, table_molecules, number_unique_molecules)


def _write_total_data(data_folder_path: Path, total_data: _TotalData) -> None:
    """Writes more info into the Readme.md file.

    Args:
        data_folder_path (Path): the data folder we are looking at.
        total_data (_TotalData): the total dataclass.
    """

    logging.info("Start writing the total data")
    with open(data_folder_path / "Readme.md", "a") as readme:
        readme.write("---  \n")
        readme.write("## Total Data  \n")
        readme.write("### General  \n")
        readme.write(f"maximun_age = {total_data.maximun_age}  \n")
        readme.write(f"median_age = {total_data.median_age:.2f}  \n")
        readme.write(f"maximum_iterations = {total_data.maximum_iterations}  \n")
        readme.write(f"number_of_children = {total_data.number_of_children}  \n")
        readme.write(
            f"number_of_total_individuals = {total_data.number_of_total_individuals}  \n"
        )
        readme.write(
            f"number_of_total_genotypes = {total_data.number_of_total_genotypes}  \n"
        )

        readme.write("### Prots  \n")
        if total_data.data_prots:
            # the prots column wasn't empty
            readme.write(
                f"maximum_prots_at_once = {total_data.data_prots.maximum_molecules}  \n"
            )
            readme.write(
                f"number_unique_prots = {total_data.data_prots.number_unique_molecules}  \n\n"
            )
            logging.debug("Writing prots table")
            readme.write(total_data.data_prots.table_molecules)
            logging.debug("Finish writing prots table")
        else:
            readme.write("This simulation didn't produce any prots.  \n")

        readme.write("\n\n### Mets  \n")
        if total_data.data_mets:
            # the mets column wasn't empty
            readme.write(
                f"maximum_mets_at_once = {total_data.data_mets.maximum_molecules}  \n"
            )
            readme.write(
                f"number_unique_mets = {total_data.data_mets.number_unique_molecules}  \n\n"
            )
            logging.debug("Writing mets table")
            readme.write(total_data.data_mets.table_molecules)
            logging.debug("Finish writing mets table")
        else:
            readme.write("This simulation didn't produce any mets.  \n")

        readme.write("\n\n### Dims  \n")
        if total_data.data_dims:
            # the dims column wasn't empty
            readme.write(
                f"maximum_dims_at_once = {total_data.data_dims.maximum_molecules}  \n"
            )
            readme.write(
                f"number_unique_dims = {total_data.data_dims.number_unique_molecules}  \n\n"
            )
            logging.debug("Writing dims table")
            readme.write(total_data.data_dims.table_molecules)
            logging.debug("Finish writing dims table")
        else:
            readme.write("This simulation didn't produce any dims.  \n")

        readme.write("\n\n### Genotypes  \n")
        readme.write(f"genotype_len = {total_data.genotype_data.genotype_len}  \n")
        readme.write(
            f"total 1s in all genotypes = {total_data.genotype_data.number_of_1}  \n"
        )
        readme.write(
            f"mean 1s in all genotypes = {total_data.genotype_data.mean_of_1}  \n"
        )
        readme.write(
            f"percentage_of_1 = {total_data.genotype_data.percentage_of_1:.2%}  \n"
        )
    logging.info("Finish writing the total data")


def _process_for_stackplot_1_0(df: pd.DataFrame) -> pd.DataFrame:

    logging.info("Start processing the data")

    logging.debug("Start counting ones in Genotypes")
    df["1_count"] = df["Genotype"].apply(lambda x: x.count("1"))
    logging.debug("Finish counting ones in Genotypes")

    genotype_size = len(df["Genotype"].iloc[0])

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

    logging.info("Finish processing the data")

    return by_iteration[["Iteration", "1s_ratio", "0s_ratio"]]


def main(data_folder_path: Path) -> None:
    logging.info("Start reading the total data in total.py")
    df = get_total_df(data_folder_path)
    logging.info("Finish reading the total data in total.py")
    total_data = _total_statistics(df)
    _write_total_data(data_folder_path, total_data)

    by_iteration = _process_for_stackplot_1_0(df)
    fig = stackplot_1_0(by_iteration, total=True)
    save_fig(
        fig,
        PlotType.TOTAL,
        data_folder_path / "graphs",
    )
    close(fig)
