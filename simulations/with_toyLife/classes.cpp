#include "classes.h"

#include <algorithm>  // std::count std::min
#include <bitset>     // std::bitset
#include <iostream>
#include <stdexcept>  // std::invalid_argument
#include <string>
#include <vector>

// my functions and variables
#include "functions/func.h"
#include "functions/globals.h"

// toyLife code
#include "toylife/helper_functions.h"
#include "toylife/toy_plugin.h"

Agent::Agent(float energy, std::string genotype, int parent) {
    if (genotype.empty()) {
        this->genotype = binaryGenerator(SIZE_GENOTYPE);
    } else {
        if (genotype.size() != SIZE_GENOTYPE)  // to prevent errors
            throw std::invalid_argument("The genotype must be of size " +
                                        std::to_string(SIZE_GENOTYPE) + ".");

        this->genotype = genotype;
    }
    this->energy = energy;
    this->age = 0;
    this->id = ID_COUNT;
    this->parent = parent;
    ID_COUNT++;
}

void Agent::print(bool printAge) {
    if (printAge)
        std::cout << "Genotype: " << genotype << ". Energy: " << energy
                  << ". Age:" << age << std::endl;
    else
        std::cout << "Genotype: " << genotype << ". Energy: " << energy
                  << std::endl;
}

bool Agent::checkEnergyReproduce() { return energy >= ENERGY_TO_REPRODUCE; }

bool Agent::checkDie() { return (energy < ENERGY_TO_DIE || age > AGE_TO_DIE); }

// PROMOTER_EXPRESSION
void Agent::promoter_expression(const ToyPlugin& toy) {
    // OUTPUT
    // PROMOTER_FIGHT GIVES THE OUTPUT STATE FROM A GIVEN SET OF OBJECTS
    // (PROTEINS (INT) AND DIMERS (BOOST::TUPLE(INT,INT,INT)) ATTENTION: IN THIS
    // VERSION OF PROMOTER_EXPRESSION, WE NEED NOT CHECK IF THE COMPETING
    // PROTEINS ARE THE SAME OR NOT AS THEY ARE ALREADY SAVED IN DIFFERENT SLOTS
    // IN THE OBJECTS MAP

    // VEC GENOTYPE
    auto vgenotype = toy.genotype_str_to_vec(
        genotype);  // transforms binary string into list of promoters and genes
    // LOOK UP THE BINDING ENERGY OF EVERY PROTEIN TO THE PROM
    std::vector<bool> promoter_active(genotype.size(), 0);

    // LOOP OVER ALL PROMOTERS
    for (int p = 0; p < vgenotype.size(); ++p) {
        int prom = vgenotype[p].first;
        std::pair<std::map<Prot, double>, std::map<Dim, double>>
            binding_energies;
        for (mapa_prot::const_iterator it_prot = prots.begin();
             it_prot != prots.end(); ++it_prot)  // for every protein in PROTS
            if (d_less(toy.prot_prom_energies[prom][it_prot->first],
                       0.0) &&  // if the protein can bind to PROM AND
                d_less(toy.prot_prom_energies[prom][it_prot->first] +
                           toy.prot_energies[it_prot->first] -
                           toy.polymerase[prom],
                       0.0))  // it can bind it more strongly than POLY
                binding_energies.first[it_prot->first] =
                    toy.prot_prom_energies[prom][it_prot->first] +
                    toy.prot_energies[it_prot->first] -
                    toy.polymerase[prom];  // we substract the binding energy of
                                           // the polymerase; if it is greater
                                           // than zero, binding energy is zero.
        for (mapa_dim::const_iterator it_dim = dims.begin();
             it_dim != dims.end(); ++it_dim)  // for every dimer in DIMS
            if (d_less(toy.dim_prom_energies[prom][it_dim->first.id],
                       0.0) &&  // if the dimer can bind to PROM AND
                d_less(toy.dim_prom_energies[prom][it_dim->first.id] +
                           toy.dim_bond_energy[it_dim->first.id] +
                           toy.prot_energies[it_dim->first.p1] +
                           toy.prot_energies[it_dim->first.p2] -
                           toy.polymerase[prom],
                       0.0))  // it can bind it more strongly than POLY
                binding_energies.second[it_dim->first] =
                    toy.dim_prom_energies[prom][it_dim->first.id] +
                    toy.dim_bond_energy[it_dim->first.id] +
                    toy.prot_energies[it_dim->first.p1] +
                    toy.prot_energies[it_dim->first.p2] -
                    toy.polymerase[prom];  // we substract the binding energy of
                                           // the polymerase; if it is greater
                                           // than zero, binding energy is zero

        // WE LOOK FOR THE MINIMUM
        while (1) {
            double min = 0.0;
            bool i_min = 0;  //"0" means the minimum is a protein; "1" means it
                             // is a dimer
            std::map<Prot, double>::const_iterator prot_min;
            std::map<Dim, double>::const_iterator dim_min;
            for (std::map<Prot, double>::const_iterator it1 =
                     binding_energies.first.begin();
                 it1 != binding_energies.first.end();
                 ++it1) {  // loop over all proteins, we save its id (i_min) and
                           // its energy (min)
                if (d_less(it1->second, min)) {
                    min = it1->second;
                    prot_min = it1;
                }  // if binding<min
            }      // for i
            for (std::map<Dim, double>::const_iterator it2 =
                     binding_energies.second.begin();
                 it2 != binding_energies.second.end();
                 ++it2) {  // loop over all dimers, we save its id (i_min) and
                           // its energy (min)
                if (d_less(it2->second, min)) {
                    min = it2->second;
                    i_min = 1;
                    dim_min = it2;
                }  // if binding<min
            }      // for i

            if (d_equal(min,
                        0.0)) {  // If the minimal is 0, no protein or dimer
                                 // binds, and it all depends on the polymerase
                promoter_active[p] = d_less(toy.polymerase[prom], 0.0);
                break;  // we exit the loop
            }

            // We check if there is more than one minimum, and eliminate the
            // repeated ones
            int count_mins = 0;
            if (!i_min) {  // if the minimum is a protein
                for (std::map<Prot, double>::const_iterator it1 =
                         std::next(prot_min);
                     it1 != binding_energies.first.end();
                     ++it1)  // we start the loop on the object following the
                             // minimal (the others have greater binding
                             // energies)
                    if (d_equal(it1->second,
                                min))  // if the binding energies are the same
                        count_mins++;  // we sum to the counter
                for (std::map<Dim, double>::const_iterator it2 =
                         binding_energies.second.begin();
                     it2 != binding_energies.second.end();
                     ++it2)  // we compare the minimum protein with the dimers
                    if (d_equal(it2->second, min))
                        count_mins++;
            }       // if minimum is a protein
            else {  // minimum is a dimer (then we don't need to check
                    // repetitions in proteins, because we examined the proteins
                    // first)
                for (std::map<Dim, double>::const_iterator it2 =
                         std::next(dim_min);
                     it2 != binding_energies.second.end();
                     ++it2)  // we start the loop on the object following the
                             // minimal (the others have greater binding
                             // energies)
                    if (d_equal(it2->second,
                                min))  // if the binding energies are the same
                        count_mins++;  // we sum to the counter
            }                          // minimum is a dimer

            if (count_mins) {  // if there are repeated energies
                for (std::map<Prot, double>::iterator it1 =
                         binding_energies.first.begin();
                     it1 != binding_energies.first.end(); ++it1)
                    if (d_equal(it1->second,
                                min))  // if the binding energies are the same
                        binding_energies.first.erase(
                            it1);  // we sum to the counter
                for (std::map<Dim, double>::iterator it2 =
                         binding_energies.second.begin();
                     it2 != binding_energies.second.end(); ++it2)
                    if (d_equal(it2->second, min))
                        binding_energies.second.erase(it2);
            } else {  // if there is only one minimum, we check if PROM is
                      // activated
                if (!i_min)
                    promoter_active[p] =
                        toy.prot_pol[prom]
                                    [prot_min->first];  // return the minimum
                else
                    promoter_active[p] =
                        toy.dim_pol[prom]
                                   [dim_min->first.id];  // return the minimum
                break;                                   // we exit the loop
            }
        }  // while TRUE
    }      // for promoters

    // EXPRESSION
    prots.clear();  // now we know which promoters are expressed, all objects
                    // disappear
    dims.clear();
    for (int i = 0; i < promoter_active.size(); ++i) {
        if (promoter_active[i]) {
            prots[toy.prot_gen[vgenotype[i].second]]++;  // we add the proteins
                                                         // that are expressed
            energy -=
                TRANSLATION_ENERGY;  // we subtract the energy from translation
        }

        return;
    }
}

// REACTING
void Agent::reacting(const ToyPlugin& toy) {
    // GIVEN A SET OF OBJECTS, IT TELLS US WHAT BINDS WHAT AND WHAT BREAKS
    // WHAT
    std::map<double, mapa_owm>
        binding_energies;  // creates a map of energies, so that complexes
                           // with the same final energy are classified in
                           // the same group. Each energy has a map of OWM,
                           // which has three members: prot, dim and met (if
                           // there is no Prot in the complex, it will be
                           // -1; if there is no Dim, it will be (-1,0,0),
                           // and if there is no Met it will be the empty
                           // string, "") and [VALUE] an int counting how
                           // many possible molecules can be formed of this
                           // type
    // First we check if any Dimer forms
    for (mapa_prot::const_iterator it1 = prots.begin(); it1 != prots.end();
         ++it1)  // we only want to explore the combinations of two proteins
        for (mapa_prot::const_iterator it2 = it1; it2 != prots.end();
             ++it2)  // remember that proteins are sorted (and that they can
                     // form dimers with themselves)
            if (it1 != it2 ||
                (it1 == it2 &&
                 it1->second >
                     1)) {  // Prots will only dimerize with themselves if
                            // there is more than one copy
                std::pair<int, int> perim =
                    std::make_pair(toy.prot_perimeters[it1->first],
                                   toy.prot_perimeters[it2->first]);
                std::map<std::pair<int, int>, int>::const_iterator it_dim =
                    toy.dim_perim.find(perim);
                if (it_dim !=
                    toy.dim_perim.end()) {  // if this pair forms a dimer,
                                            // we form the tuple
                    double b_energy =
                        toy.dim_bond_energy[it_dim->second] +
                        toy.prot_energies[it1->first] +
                        toy.prot_energies[it2->first];  // energy of the
                                                        // complex
                    Dim new_dim(it_dim->second, it1->first, it2->first);
                    OWM new_complex(-1, new_dim, Met());
                    binding_energies[b_energy][new_complex] =
                        int_min(it1->second, it2->second);
                }  // if a dimer can be formed
            }      // if it1!=it2

    // Then we check if any Protein binds any Met
    for (mapa_prot::const_iterator it_prot = prots.begin();
         it_prot != prots.end(); ++it_prot)
        for (mapa_met::const_iterator it_met = mets.begin();
             it_met != mets.end(); ++it_met) {
            pairPM par1 = std::make_pair(it_met->first, it_prot->first);
            try {  // see if the prot binds the met
                // std::map<Met, std::map<Prot,double> >::const_iterator
                // iter_tuplita=prot_met.find(it_met->first);
                // std::map<Prot,double> tuplita=iter_tuplita->second;
                double e1 = toy.prot_met.at(par1);
                if (d_less(e1,
                           0.0)) {  // if the protein can bind the metabolite
                    double b_energy =
                        e1 + toy.prot_energies[it_prot->first];  // energy of
                                                                 // the complex
                    OWM new_complex(it_prot->first, Dim(), it_met->first);
                    binding_energies[b_energy][new_complex] =
                        int_min(it_prot->second, it_met->second);
                }
            } catch (std::out_of_range) {
            }  // if it doesn't, don't do anything
        }

    // Then we check if any Dimer binds any Met
    for (mapa_dim::const_iterator it_dim = dims.begin(); it_dim != dims.end();
         ++it_dim)
        for (mapa_met::const_iterator it_met = mets.begin();
             it_met != mets.end(); ++it_met) {
            pairDM par1 = std::make_pair(it_met->first, it_dim->first.id);
            try {  // look and see if the dimer binds to the met
                Dmet tupla = toy.dim_met.at(par1);
                if (d_less(tupla.eg,
                           0.0)) {  // if the dimer can bind the metabolite
                    double b_energy =
                        tupla.eg + toy.dim_bond_energy[it_dim->first.id] +
                        toy.prot_energies[it_dim->first.p1] +
                        toy.prot_energies[it_dim->first.p2];  // energy of
                                                              // the complex
                    OWM new_complex(-1, it_dim->first, it_met->first);
                    binding_energies[b_energy][new_complex] =
                        int_min(it_dim->second, it_met->second);
                }
            } catch (std::out_of_range) {
            }  // if it doesn't don't do anything
        }

    // Finally, we check if any D+M breaks
    for (mapa_owm::const_iterator it_dm = owns.begin(); it_dm != owns.end();
         ++it_dm) {
        if (it_dm->first.dim.id == -1)
            continue;  // this means it's a P+M complex
        for (mapa_prot::const_iterator it_prot = prots.begin();
             it_prot != prots.end(); ++it_prot) {
            pairDM par1 = std::make_pair(it_dm->first.met, it_dm->first.dim.id);
            Dmet tupla = toy.dim_met.at(par1);
            if (tupla.seq.size() != 16)
                continue;  // only check this if the D+M can actually be
                           // broken!
            std::pair<int, std::string> par2(it_prot->first, tupla.seq);
            try {  // look and see if the protein can break this dimer
                double b_energy = toy.prot_breaking.at(par2).first +
                                  toy.prot_energies[it_prot->first];
                int c_min = toy.prot_breaking.at(par2).second;
                if (c_min <
                    2)  // the sequences should be ordered in such a way
                        // that the first half is always the first protein
                    b_energy +=
                        toy.prot_energies[it_dm->first.dim.p1];  // sum energy
                                                                 // of first
                                                                 // subunit
                // if ((c_min<2 && tupla.bind<3) ||//if the prot binds to
                // the first subunit when the dimer is not reversed
                //     (c_min>1 && tupla.bind>2))//or if it binds to the
                //     second subunit when the dimer is reversed
                //   b_energy += prot_energies[it_dm->first.dim.p1];//sum
                //   energy of first subunit
                else
                    b_energy +=
                        toy.prot_energies[it_dm->first.dim.p2];  // sum energy
                                                                 // of second
                                                                 // subunit
                double met_energy =
                    toy.dim_bond_energy[it_dm->first.dim.id] +
                    toy.prot_energies[it_dm->first.dim.p1] +
                    toy.prot_energies[it_dm->first.dim.p2];  // E of the
                                                             // existing complex
                if (d_less(b_energy, met_energy)) {  // then it could break
                    OWM new_complex(it_prot->first, it_dm->first.dim,
                                    it_dm->first.met);
                    binding_energies[b_energy][new_complex] =
                        int_min(it_prot->second, it_dm->second);
                }
            }  // try
            catch (std::out_of_range) {
            }  // it doesn't bind to this sequence
        }      // for prots
    }          // for d+m complexes

    // NOW WE HAVE A MAP OF ENERGIES
    // LOOP OVER EACH ENERGY AND SEE WHAT HAPPENS
    for (std::map<double, mapa_owm>::iterator it = binding_energies.begin();
         it != binding_energies.end(); ++it) {
        // std::cout << "Energy! " << it->first << std::endl;
        mapa_prot number_prots;  // will hold how many complexes each
                                 // protein is involved in (if it's greater
                                 // than prots[i], then no dimers is formed
        mapa_dim number_dims;    // will hold how many complexes each protein
                                 // is involved in (if it's greater than
                                 // prots[i], then no dimers is formed
        mapa_met number_mets;    // will hold how many complexes each protein
                                 // is involved in (if it's greater than
                                 // prots[i], then no dimers is formed
        mapa_owm number_dm;      // because std::map doesn't work well with
                                 // boost::tuple, this vector will serve as an
                                 // index for the elements in owns;
        for (mapa_owm::iterator it_tuple = it->second.begin();
             it_tuple != it->second.end(); ++it_tuple)  // for each complex
            if (it_tuple->first.met.empty()) {  // protein-protein complex
                number_prots[it_tuple->first.dim.p1] += it_tuple->second;
                number_prots[it_tuple->first.dim.p2] += it_tuple->second;
            } else if (it_tuple->first.prot !=
                       -1) {  // there are proteins and mets
                if (it_tuple->first.dim
                        .empty()) {  // protein-met complex (no dimers)
                    number_prots[it_tuple->first.prot] += it_tuple->second;
                    number_mets[it_tuple->first.met] += it_tuple->second;
                } else {  // protein breaking a d+m complex
                    number_prots[it_tuple->first.prot] += it_tuple->second;
                    number_dm[OWM(-1, it_tuple->first.dim,
                                  it_tuple->first.met)] += it_tuple->second;
                }
            }       // proteins and mets are involved
            else {  // dimer-met complex
                number_dims[it_tuple->first.dim] += it_tuple->second;
                number_mets[it_tuple->first.met] += it_tuple->second;
            }
        // NOW, FOR EVERY POSSIBLE COMPLEX, WE CHECK IF THE UNITS ARE
        // INVOLVED IN TOO MANY COMPLEXES AND DISCARD THOSE
        for (mapa_owm::iterator it_tuple = it->second.begin();
             it_tuple != it->second.end(); ++it_tuple)  // for each complex
            if (it_tuple->first.met.empty()) {  // protein-protein complex
                if (prots[it_tuple->first.dim.p1] <
                        number_prots[it_tuple->first.dim.p1] ||  // prot1 more
                                                                 // involved
                    prots[it_tuple->first.dim.p2] <
                        number_prots[it_tuple->first.dim.p2])  // prot2 more
                                                               // involved
                    it->second.erase(it_tuple);  // this dimer will not form
            } else if (it_tuple->first.prot !=
                       -1) {  // proteins and mets are involved
                if (it_tuple->first.dim.empty()) {  // protein-met complex
                    if (prots[it_tuple->first.prot] <
                            number_prots[it_tuple->first.prot] ||  // proteins
                                                                   // more
                                                                   // involved
                        mets[it_tuple->first.met] <
                            number_mets[it_tuple->first.met])  // met more
                                                               // involved
                        it->second.erase(it_tuple);  // this P+M will not form
                } else {  // protein breaking a d+m complex
                    OWM new_tuple(-1, it_tuple->first.dim, it_tuple->first.met);
                    if (prots[it_tuple->first.prot] <
                            number_prots[it_tuple->first.prot] ||  // protein
                                                                   // more
                                                                   // involved
                        owns[new_tuple] <
                            number_dm[new_tuple])  // d+m more involved
                        it->second.erase(
                            it_tuple);  // this D+M will not be broken
                }
            }       // proteins and mets are involved
            else {  // dimer-met complex
                if (dims[it_tuple->first.dim] <
                        number_dims[it_tuple->first.dim] ||  // dimer more
                                                             // involved
                    mets[it_tuple->first.met] <
                        number_mets[it_tuple->first.met])  // met more
                                                           // involved
                    it->second.erase(it_tuple);  // this D+M will not form
            }
        // NOW WE FORM THE COMPLEXES THAT REMAIN
        for (mapa_owm::const_iterator it_tuple = it->second.begin();
             it_tuple != it->second.end(); ++it_tuple)  // for each complex
            if (it_tuple->first.met.empty()) {  // protein-protein complex
                dims[it_tuple->first.dim] +=
                    it_tuple->second;  // we add as many copies as possible
                prots[it_tuple->first.dim.p1] -=
                    it_tuple->second;  // we eliminate the Prot copies
                prots[it_tuple->first.dim.p2] -= it_tuple->second;
            } else if (it_tuple->first.prot !=
                       -1) {  // there are proteins and mets
                if (it_tuple->first.dim.empty()) {  // protein-met complex
                    owns[it_tuple->first] += it_tuple->second;
                    prots[it_tuple->first.prot] -= it_tuple->second;
                    mets[it_tuple->first.met] -= it_tuple->second;
                } else {  // protein breaking a d+m complex
                    owns[OWM(-1, it_tuple->first.dim, it_tuple->first.met)] -=
                        it_tuple->second;
                    prots[it_tuple->first.prot] -= it_tuple->second;
                    energy += BREAKING_ENERGY;  // it->first?
                    // Now we add the rests of the mets to Met
                    pairDM par1 = std::make_pair(it_tuple->first.met,
                                                 it_tuple->first.dim.id);
                    Dmet tupla = toy.dim_met.at(par1);
                    std::string pmet1 = it_tuple->first.met.substr(
                        0,
                        tupla.pos);  // we split the met in the two parts
                                     // defined by the Dimer when it binds
                    std::string pmet2 =
                        it_tuple->first.met.substr(pmet1.size());
                    mets[pmet1] += it_tuple->second;
                    mets[pmet2] += it_tuple->second;
                }
            }       // proteins and mets
            else {  // dimer-met complex (it should check this: if
                    // (boost::get<0>(boost::get<1>(*it_tuple))!=-1)
                owns[it_tuple->first] += it_tuple->second;
                dims[it_tuple->first.dim] -= it_tuple->second;
                mets[it_tuple->first.met] -= it_tuple->second;
            }
    }  // for all energies

    // NOW WE ELIMINATE THE ELEMENTS THAT HAVE BEEN USED UP
    for (mapa_prot::iterator it_prot = prots.begin(); it_prot != prots.end();
         ++it_prot)
        if (it_prot->second == 0)
            prots.erase(it_prot);  // eliminate the proteins that have 0 copies

    for (mapa_dim::iterator it_dim = dims.begin(); it_dim != dims.end();
         ++it_dim)
        if (it_dim->second == 0)
            dims.erase(it_dim);  // eliminate dimers that have 0 copies

    for (mapa_met::iterator it_met = mets.begin(); it_met != mets.end();
         ++it_met)
        if (it_met->second == 0)
            mets.erase(it_met);  // eliminate mets that have 0 copies

    for (mapa_owm::iterator it_owm = owns.begin(); it_owm != owns.end();
         ++it_owm)
        if (it_owm->second == 0)
            owns.erase(it_owm);

    return;
}

void Agent::eat(std::map<std::string, int>& food, const ToyPlugin& toy) {
    /**
     * Describe the interaction between an Agent and the food.
     *We make an AND operation between the two binary strings and
     *the energy gained by the Agent is the number of '1' that the
     *final string has.
     * @param food The food that the Agent is going to eat.
     */
    // START THE CYCLE: FOOD ENTERS THE CELL
    for (auto it = food.begin(); it != food.end(); ++it) {
        mets[it->first] += it->second;
        food.at(it->first) = 0;  // not sure about this
    }
    // NOW EXISTING PROTS AND METS TRY TO BIND THE METS
    reacting(toy);
    // THEN REGULATION OCCURS
    promoter_expression(toy);
    // NOW THE NEW PROTEINS WILL DIMERIZE IN THE PRESENCE OF THE
    // METABOLITE-COMPOUND IF SOMETHING IS BROKEN, THEN THE CELL GAINS
    // ENERGY
    reacting(toy);
    // At the end of the cycle, all metabolites that haven't been bound by
    // anything disappear and return to the environment
    for (auto it = mets.begin(); it != mets.end(); ++it)
        food[it->first] += it->second;
    mets.clear();
    // Now the P+M and D+M will become part of Met too (the Ps and Ds will
    // disappear)
    for (auto it = owns.begin(); it != owns.end(); ++it)
        mets[it->first.met] += it->second;
    owns.clear();  // and all objects with mets will disappear of course
    return;
}

std::vector<std::string> Agent::getAgentData() {
    // TODO add here the data of the mets, dims and prots
    std::vector<std::string> data(5);
    data[0] = std::to_string(id);
    data[1] = genotype;
    data[2] = std::to_string(energy);
    data[3] = std::to_string(age);
    data[4] = std::to_string(parent);

    return data;
}

std::pair<Agent, Agent> divide(Agent parent, float p) {
    /**
     * Divide an agent into two agents.
     * @param parent The agent that is going to be divided.
     * @param p The probability of the children of being mutated.
     */

    float energyC = parent.energy / 2;
    std::string genotype1 = mutate(parent.genotype, p);
    std::string genotype2 = mutate(parent.genotype, p);

    Agent child1(energyC, genotype1, parent.id);
    Agent child2(energyC, genotype2, parent.id);

    return std::make_pair(child1, child2);
}

// POPULATION
Population::Population(int sizePopulation) {
    this->sizePopulation = sizePopulation;
    this->agents = std::vector<Agent>();
    std::vector<float> energies = createRandomArray(sizePopulation, 15);
    for (int i = 0; i < sizePopulation; ++i) {
        Agent agent(energies[i]);
        agents.push_back(agent);
    }
}

void Population::print() {
    std::cout << "You are printing the whole population: \n";
    for (int i = 0; i < sizePopulation; ++i) {
        agents[i].print();
        std::cout << "\n";
    }
}

void Population::iteration(std::vector<std::string> food, float cost,
                           double costVariation, bool print) {
    /**
     * Here we do the iteration of the population. First we
     *make the agents search for food. Then we generate a random
     *order to eat the food. Then we check the energy of each agent
     *and do the corresponding action. First we check the ones that died.
     *Then we check the ones that can reproduce and we reproduce them.
     * @param food The food that the agents are going to eat.
     * @param cost The cost of searching for food.
     * @param costVariation The variation of the cost so every agent
     *                      doesn't spend the same energy searching for
     *food.
     * @param print If true it logs extra information.
     */

    int sizeFood = food.size();

    std::random_device rd;
    std::mt19937 g(rd());
    // std::uniform_real_distribution<double> variationDist(
    //     -cost * costVariation, cost * costVariation);

    // for (Agent& i : agents) {  // every agent search for food
    //     double randomCost = cost + variationDist(g);
    //     i.energy -= randomCost;
    // }

    if ((sizePopulation > sizeFood) && print)
        std::cout << "There is not enough food for everybody.\n";

    int smaller = std::min(sizePopulation, sizeFood);

    // generate random order
    std::vector<int> order(smaller);
    std::iota(order.begin(), order.end(), 0);
    std::shuffle(order.begin(), order.end(), g);

    std::vector<float>
        gainedEnergies;  // store the energies gained by each agent

    for (int i = 0; i < smaller; ++i) {
        Agent& agent = agents[order[i]];

        if (agent.checkDie())  // When searching for food the agent
                               // died.
            continue;
        // gainedEnergies.push_back(agent.eat(food[i]));
    }

    // Fill the vectors for statistics.
    int sum = std::accumulate(gainedEnergies.begin(), gainedEnergies.end(), 0);
    averageEnergyGain.push_back(static_cast<double>(sum) / (float)smaller);

    auto maxGain =
        std::max_element(gainedEnergies.begin(), gainedEnergies.end());
    maxEnergyGain.push_back(*maxGain);

    auto minGain =
        std::min_element(gainedEnergies.begin(), gainedEnergies.end());
    minEnergyGain.push_back(*minGain);

    generation++;
    addAges();
}

void Population::afterIteration(float p) {
    /**
     * Here we check the energy of each agent and do the
     *corresponding action. First we check the ones that died.
     *Then we check the ones that can reproduce and we reproduce them.
     * @param p The probability of mutation.
     */

    std::vector<int> deadsPositions;  // store the indexes of the deads

    for (int i = 0; i < sizePopulation; ++i) {
        if (agents[i].checkDie())
            deadsPositions.push_back(i);
    }

    deleteElements(agents, deadsPositions);

    std::vector<int>
        reproducePositions;  // store the indexes of the ones to reproduce
    for (int i = 0; i < sizePopulation; ++i) {
        if (agents[i].checkEnergyReproduce())
            reproducePositions.push_back(i);
    }

    std::vector<Agent> newElements(reproducePositions.size() * 2, Agent(0));
    for (int i = 0; i < reproducePositions.size(); ++i) {
        std::pair<Agent, Agent> children =
            divide(agents[reproducePositions[i]], p);
        newElements[i * 2] = children.first;
        newElements[i * 2 + 1] = children.second;
    }

    deleteElements(agents, reproducePositions);
    agents.insert(agents.end(), newElements.begin(), newElements.end());
    this->sizePopulation = agents.size();  // update the size of the population
}

void Population::deleteElements(std::vector<Agent>& agents,
                                std::vector<int> indexes) {
    for (auto it = indexes.rbegin(); it != indexes.rend(); it++)
        agents.erase(agents.begin() + *it);

    this->sizePopulation = agents.size();  // update the size of the population
}

void Population::addAges() {
    for (Agent& i : agents)
        i.age++;
}

std::vector<std::string> Population::getGenotypes() {
    std::vector<std::string> genotypes(sizePopulation);
    for (int i = 0; i < sizePopulation; ++i)
        genotypes[i] = agents[i].genotype;

    return genotypes;
}

std::vector<std::vector<std::string>> Population::getPopulationData() {
    std::vector<std::vector<std::string>> data(sizePopulation);
    for (int i = 0; i < sizePopulation; ++i)
        data[i] = agents[i].getAgentData();

    return data;
}
