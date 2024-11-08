#include "classes.h"

// my functions and variables
#include "functions/func.h"
#include "functions/globals.h"

// toyLife code
#include "toylife/helper_functions.h"
#include "toylife/toy_plugin.h"

Agent::Agent(float energy, std::string genotype, int parent, int sizeGenotype,
             mapa_prot prots, mapa_dim dims, mapa_met mets) {
    if (genotype.empty()) {
        this->genotype = binaryGenerator(sizeGenotype, 0.5f);
    } else {
        if (genotype.size() != sizeGenotype)  // to prevent errors
            throw std::invalid_argument("The genotype must be of size " +
                                        std::to_string(sizeGenotype) + ".");

        this->genotype = genotype;
    }
    this->energy = energy;
    this->age = 0;
    this->id = ID_COUNT;
    this->parent = parent;
    this->prots = prots;
    this->dims = dims;
    this->mets = mets;
    ID_COUNT++;
}

void Agent::print(bool complete) {
    std::cout << "ID: " << id << ". Energy: " << energy << ". Age: " << age
              << ". Parent: " << parent << std::endl;

    if (complete) {
        std::cout << "Genotype: " << genotype << "\n";
        std::cout << "Prots: \n";
        printMap(prots);
        std::cout << "Dims: \n";
        printMap(dims);
        std::cout << "Mets: \n";
        printMap(mets);
        std::cout << "Owns: \n";
        printMap(owns);
    }
}

bool Agent::checkEnergyReproduce() { return energy >= ENERGY_TO_REPRODUCE; }

bool Agent::checkDie() { return (energy < ENERGY_TO_DIE || age > AGE_TO_DIE); }

bool Agent::checkPDM() {
    return (prots.empty() && dims.empty() && mets.empty());
}

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

    // std::cout << "Genes " << std::endl;
    // for (auto it=vgenotype.begin(); it!=vgenotype.end(); ++it)
    //   std::cout << (*it).first << "\t" << toy.prot_gen[(*it).second] <<
    //   std::endl;
    // std::getchar();
    std::vector<bool> promoter_active(vgenotype.size(), 0);

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
                    toy.polymerase[prom];  // we subtract the binding energy of
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
                    toy.polymerase[prom];  // we subtract the binding energy of
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
            }  // for i
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
            }  // for i

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
            }  // if minimum is a protein
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
            }  // minimum is a dimer

            if (count_mins) {  // if there are repeated energies
                for (auto it1 = binding_energies.first.begin();
                     it1 != binding_energies.first.end();) {
                    if (d_equal(it1->second,
                                min))  // if the binding energies are the same
                        it1 = binding_energies.first.erase(it1);
                    else
                        ++it1;  // we sum to the counter
                }
                for (auto it2 = binding_energies.second.begin();
                     it2 != binding_energies.second.end();) {
                    if (d_equal(it2->second, min))
                        it2 = binding_energies.second.erase(it2);
                    else
                        ++it2;
                }

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
    }  // for promoters

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
    }

    return;
}

class DoubleComparator {
   public:
    bool operator()(const double lhs, const double rhs) const {
        // Check if the absolute difference is less than the threshold
        // Note: This is the comparison for ordering, not for equality
        // The map still requires a strict weak ordering
        double thr = 0.01;
        return lhs < rhs && std::abs(lhs - rhs) >= thr;
    }
};

// REACTING
void Agent::reacting(const ToyPlugin& toy) {
    // GIVEN A SET OF OBJECTS, IT TELLS US WHAT BINDS WHAT AND WHAT BREAKS
    // WHAT
    std::map<double, mapa_owm, DoubleComparator>
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
            }  // if it1!=it2

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
        }  // for prots
    }  // for d+m complexes

    // NOW WE HAVE A MAP OF ENERGIES
    // LOOP OVER EACH ENERGY AND SEE WHAT HAPPENS
    for (auto it = binding_energies.begin(); it != binding_energies.end();
         ++it) {
        // std::cout << it->first << std::endl;
        // printMap(it->second);// << std::endl;
        // std::getchar();
        //  std::cout << "Energy! " << it->first << std::endl;
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
            }  // proteins and mets are involved
            else {  // dimer-met complex
                number_dims[it_tuple->first.dim] += it_tuple->second;
                number_mets[it_tuple->first.met] += it_tuple->second;
            }

        // NOW, FOR EVERY POSSIBLE COMPLEX, WE CHECK IF THE UNITS ARE
        // INVOLVED IN TOO MANY COMPLEXES AND DISCARD THOSE
        for (mapa_owm::iterator it_tuple = it->second.begin();
             it_tuple != it->second.end();) {   // for each complex
            if (it_tuple->first.met.empty()) {  // protein-protein complex
                if (prots[it_tuple->first.dim.p1] <
                        number_prots[it_tuple->first.dim.p1] ||  // prot1 more
                                                                 // involved
                    prots[it_tuple->first.dim.p2] <
                        number_prots[it_tuple->first.dim.p2]) {  // prot2 more
                                                                 // involved
                    it_tuple =
                        it->second.erase(it_tuple);  // this dimer will not form
                } else
                    ++it_tuple;
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
                        it_tuple = it->second.erase(
                            it_tuple);  // this P+M will not form
                    else
                        ++it_tuple;
                } else {  // protein breaking a d+m complex
                    OWM new_tuple(-1, it_tuple->first.dim, it_tuple->first.met);
                    if (prots[it_tuple->first.prot] <
                            number_prots[it_tuple->first
                                             .prot] ||  // protein more involved
                        owns[new_tuple] <
                            number_dm[new_tuple])  // d+m more involved
                        it_tuple = it->second.erase(
                            it_tuple);  // this D+M will not be broken
                    else
                        ++it_tuple;
                }
            }  // proteins and mets are involved
            else {  // dimer-met complex
                if (dims[it_tuple->first.dim] <
                        number_dims[it_tuple->first.dim] ||  // dimer more
                                                             // involved
                    mets[it_tuple->first.met] <
                        number_mets[it_tuple->first.met])  // met more involved
                    it_tuple =
                        it->second.erase(it_tuple);  // this D+M will not form
                else
                    ++it_tuple;
            }
        }

        // NOW WE FORM THE COMPLEXES THAT REMAIN
        for (mapa_owm::const_iterator it_tuple = it->second.begin();
             it_tuple != it->second.end(); ++it_tuple) {  // for each complex
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
                        0, tupla.pos);  // we split the met in the two parts
                                        // defined by the Dimer when it binds
                    std::string pmet2 =
                        it_tuple->first.met.substr(pmet1.size());
                    mets[pmet1] += it_tuple->second;
                    mets[pmet2] += it_tuple->second;
                }
            }  // proteins and mets
            else {  // dimer-met complex (it should check this: if
                // (boost::get<0>(boost::get<1>(*it_tuple))!=-1)
                owns[it_tuple->first] += it_tuple->second;
                dims[it_tuple->first.dim] -= it_tuple->second;
                mets[it_tuple->first.met] -= it_tuple->second;
            }
        }  // for each complex
    }  // for all energies

    // NOW WE ELIMINATE THE ELEMENTS THAT HAVE BEEN USED UP
    for (auto it_prot = prots.begin(); it_prot != prots.end();)
        if (it_prot->second == 0)
            it_prot = prots.erase(
                it_prot);  // eliminate the proteins that have 0 copies
        else
            ++it_prot;

    for (auto it_dim = dims.begin(); it_dim != dims.end();) {
        if (it_dim->second == 0)
            it_dim = dims.erase(it_dim);  // eliminate dimers that have 0 copies
        else
            ++it_dim;
    }

    for (auto it_met = mets.begin(); it_met != mets.end();)
        if (it_met->second == 0)
            it_met = mets.erase(it_met);  // eliminate mets that have 0 copies
        else
            ++it_met;

    for (auto it_owm = owns.begin(); it_owm != owns.end();)
        if (it_owm->second == 0)
            it_owm = owns.erase(it_owm);
        else
            ++it_owm;

    return;
}

float Agent::eat(std::map<std::string, int>& food, const ToyPlugin& toy) {
    /**
     * Describe the interaction between an Agent and the food.
     * @param food The food that the Agent is going to eat.
     */

    float previous_energy = energy;
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
    return energy - previous_energy;
}

bool Agent::metabolism(std::map<std::string, int>& food, const ToyPlugin& toy) {
    int gene_number = genotype.size() / 20;
    int maxlength = std::pow(2, gene_number);
    double e0 = energy;
    for (int i = 0; i < maxlength; ++i) {
        eat(food, toy);
        // std::cout << energy << std::endl;
        if (energy > e0)
            return 1;
    }
    if (energy > e0)
        return 1;
    else
        return 0;
}

std::vector<std::string> Agent::getAgentData() {
    // TODO add here the data of the mets, dims and prots
    std::vector<std::string> data(8);
    data[0] = std::to_string(id);
    data[1] = genotype;
    data[2] = std::to_string(energy);
    data[3] = std::to_string(age);
    data[4] = std::to_string(parent);
    data[5] = fromMapToString(prots);
    data[6] = fromMapToString(mets);
    data[7] = fromMapToString(dims);

    return data;
}

std::pair<Agent, Agent> divide(Agent parent) {
    /**
     * Divide an agent into two agents.
     * @param parent The agent that is going to be divided.
     * @param p The probability of the children of being mutated.
     */

    float energyC = parent.energy / 2;
    std::string genotype1 = mutate(parent.genotype);
    std::string genotype2 = mutate(parent.genotype);

    Agent child1(energyC, genotype1, parent.id);
    Agent child2(energyC, genotype2, parent.id);

    if (parent.checkPDM()) {
        // It doesn't have ane Prots, Mets or Dims

        return std::make_pair(child1, child2);
    }

    // Now we star the process of random distribution of the objects that the
    // parent might have. We shuffle the prots, mets and dims like a deck of
    // cards and deal them to the childs.
    if (!parent.prots.empty()) {
        std::pair<mapa_prot, mapa_prot> child_prots =
            mapDistribution(parent.prots);
        child1.prots = child_prots.first;
        child2.prots = child_prots.second;
    }

    if (!parent.mets.empty()) {
        std::pair<mapa_met, mapa_met> child_mets = mapDistribution(parent.mets);
        // we change the dealing order
        child1.mets = child_mets.second;
        child2.mets = child_mets.first;
    }

    if (!parent.dims.empty()) {
        std::pair<mapa_dim, mapa_dim> child_dims = mapDistribution(parent.dims);
        child1.dims = child_dims.first;
        child2.dims = child_dims.second;
    }

    return std::make_pair(child1, child2);
}

// POPULATION
Population::Population(int sizePopulation) {
    this->sizePopulation = sizePopulation;
    this->agents = std::vector<Agent>();
    std::vector<float> energies =
        createRandomArray(sizePopulation, ENERGY_TO_REPRODUCE * 1.25);
    for (int i = 0; i < sizePopulation; ++i) {
        Agent agent(energies[i]);
        agents.push_back(agent);
    }

    this->averageEnergyGain.reserve(NUMBER_OF_GENERATIONS);
    this->totalEnergyGain.reserve(NUMBER_OF_GENERATIONS);
    this->maxEnergyGain.reserve(NUMBER_OF_GENERATIONS);
    this->minEnergyGain.reserve(NUMBER_OF_GENERATIONS);
}

void Population::print(bool complete) {
    std::cout << "Population of size: " << sizePopulation << "\n";
    for (int i = 0; i < sizePopulation; ++i) {
        agents[i].print(complete);
        std::cout << "\n";
    }
}

std::vector<std::map<std::string, int>> Population::iteration(
    std::map<std::string, int>& foodMap, const ToyPlugin& toy, bool print,
    const int& iterationNumber) {
    /**
     * Here we do the iteration of the population. First we generate a random
     * order for the agents to eat so not always the same are eating.
     * Then we start iterating over all the agents. We take a sample of food
     * from our foodMap and give it to the agent.
     * We store the difference in energy between before and after eating. We
     * also store the food (metabolites) that the agent returned to the
     * environment.
     * After all the agents have eaten we calculate the statistics of the
     * iteration, add 1 to the generation counter and also add 1 to all the ages
     * of the agents.

     * @param foodMap The food that the agents are going to eat.
     * @param toy The toyLife object.
     */

    // generate random order
    std::vector<int> order(sizePopulation);
    std::iota(order.begin(), order.end(), 0);
    std::shuffle(order.begin(), order.end(), GENERATOR);

    // store the energies gained by each agent
    float sum = 0.0f;
    float max = 0.0f;
    float min = 0.0f;

    int tenPercent = sizePopulation / 10;

    std::vector<std::map<std::string, int>> returnedFood(sizePopulation);

    for (int i = 0; i < sizePopulation; ++i) {
        Agent& agent = agents[order[i]];

        // this resets the map every iteration
        std::map<std::string, int> food2eat = sampleFood(foodMap);

        float energyGain = agent.eat(food2eat, toy);
        sum += energyGain;
        if (energyGain > max)
            max = energyGain;
        if (energyGain < min)
            min = energyGain;

        // PRINT INFORMATION
        if (print) {
            if (tenPercent == 0 & i + 1 == sizePopulation) {
                std::cout << currentTime() << "   In iteration "
                          << iterationNumber
                          << " . Progress:" << i / tenPercent * 10
                          << "%  Processed Agents: " << i + 1 << "\n";
            } else if (tenPercent > 0 & i % tenPercent == 0) {
                std::cout << currentTime() << "   In iteration "
                          << iterationNumber
                          << " . Progress:" << i / tenPercent * 10
                          << "%  Processed Agents: " << i + 1 << "\n";
            }
        }

        // store the food that the agent return to the environment
        returnedFood[i] = food2eat;
    }

    //* Fill the vectors for statistics.
    averageEnergyGain.push_back(static_cast<double>(sum) /
                                (float)sizePopulation);
    totalEnergyGain.push_back(sum);

    maxEnergyGain.push_back(max);

    minEnergyGain.push_back(min);

    generation++;
    addAges();

    return returnedFood;
}

void Population::afterIteration(bool print) {
    /**
     * Here we check the energy of each agent and do the
     *corresponding action. First we check the ones that died.
     *Then we check the ones that can reproduce and we reproduce them.
     */
    std::vector<int> deadsPositions;  // store the indexes of the deads

    if (print) {
        std::cout << currentTime() << "   getting deads\n";
    }
    for (int i = 0; i < sizePopulation; ++i) {
        if (agents[i].checkDie())
            deadsPositions.push_back(i);
    }
    if (print) {
        std::cout << currentTime() << "   got deads\n";
    }

    numberOfDeads.push_back(deadsPositions.size());
    if (print) {
        std::cout << currentTime() << "   deleting deads\n";
    }
    deleteElements(agents, deadsPositions);
    if (print) {
        std::cout << currentTime() << "   deleted deads\n";
    }

    if (print) {
        std::cout << currentTime() << "   getting reproductions\n";
    }
    std::vector<int>
        reproducePositions;  // store the indexes of the ones to reproduce
    for (int i = 0; i < sizePopulation; ++i) {
        if (agents[i].checkEnergyReproduce())
            reproducePositions.push_back(i);
    }

    if (print) {
        std::cout << currentTime() << "   got reproductions\n";
    }

    numberOfReproductions.push_back(reproducePositions.size());
    if (print) {
        std::cout << currentTime() << "   reproducing\n";
    }
    if (!reproducePositions.empty()) {
        // if not empty
        std::vector<Agent> newElements(reproducePositions.size() * 2, Agent(0));
        for (int i = 0; i < reproducePositions.size(); ++i) {
            std::pair<Agent, Agent> children =
                divide(agents[reproducePositions[i]]);
            newElements[i * 2] = children.first;
            newElements[i * 2 + 1] = children.second;
        }
        deleteElements(agents, reproducePositions);
        agents.insert(agents.end(), newElements.begin(), newElements.end());
    }

    if (print) {
        std::cout << currentTime() << "   reproduced\n";
    }

    this->sizePopulation = agents.size();  // update the size of the population
}

void Population::deleteElements(std::vector<Agent>& agents,
                                const std::vector<int>& indexes) {
    for (auto it = indexes.rbegin(); it != indexes.rend(); it++)
        eraseFast(agents, agents.begin() + *it);
    // agents.erase(agents.begin() + *it);

    this->sizePopulation = agents.size();  // update the size of the population
}

std::vector<Agent>::iterator Population::eraseFast(
    std::vector<Agent>& c, std::vector<Agent>::iterator it) {
    if (&(*it) == &(c.back())) {
        c.pop_back();
        return std::end(c);
    }
    *it = std::move(c.back());
    c.pop_back();
    return it;
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

std::vector<std::string> Population::getPopulationEnergy() {
    /**
     * This returns the maximum, average and minimum energy of the population at
     * that time.
     */
    std::vector<float> energies(sizePopulation);

    std::vector<std::string> results(3);

    for (int i = 0; i < sizePopulation; ++i)
        energies[i] = agents[i].energy;

    auto maxEnergy = std::max_element(energies.begin(), energies.end());
    results[0] = std::to_string(*maxEnergy);

    int sum = std::accumulate(energies.begin(), energies.end(), 0);
    results[1] =
        std::to_string(static_cast<double>(sum) / (float)sizePopulation);

    auto minEnergy = std::min_element(energies.begin(), energies.end());
    results[2] = std::to_string(*minEnergy);

    return results;
}

std::vector<std::vector<std::string>> Population::getPopulationGains() {
    /**
     * This transforms the vectors of gains into a matrix of strings.
     */
    std::vector<std::vector<std::string>> gainedEnergy(maxEnergyGain.size());

    for (size_t i = 0; i < maxEnergyGain.size(); ++i) {
        gainedEnergy[i] = {std::to_string(maxEnergyGain[i]),
                           std::to_string(averageEnergyGain[i]),
                           std::to_string(totalEnergyGain[i]),
                           std::to_string(minEnergyGain[i])};
    }
    return gainedEnergy;
}

std::vector<std::vector<std::string>> Population::getDeadsAndReproduces() {
    /**
     * This joins the numberOfDeads and numberOfReproductions vectors into a
     * writable table.
     */
    std::vector<std::vector<std::string>> deadsReproduce(numberOfDeads.size());

    for (size_t i = 0; i < numberOfDeads.size(); ++i) {
        deadsReproduce[i] = {std::to_string(numberOfDeads[i]),
                             std::to_string(numberOfReproductions[i])};
    }
    return deadsReproduce;
}
