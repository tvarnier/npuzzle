#include "Puzzle.hpp"

/*****
** Compare function for Queue
*****/
bool compare::operator() (State *a, State *b) const
{
    return (*a < *b);
}

// ----- Constructor ----- //

Puzzle::Puzzle() : m_start(new State()), m_target(new State())
{
    Puzzle::reset();
}

// ----- Destructor ----- //

Puzzle::~Puzzle()
{
    if (m_start)
        delete (m_start);
    if (m_target)
        delete (m_target);
    for (auto i = m_list.begin(); i != m_list.end(); ++i)
        delete (i->second);
}

// ----- Public functions ----- //

/*****
** Reset Options and Infos
*****/
void            Puzzle::reset() {
    m_options.algorithm = A_STAR;
    m_options.heuristic = H_MANHATTAN;
    m_options.generation = false;
    m_options.generation_length = 0;
    m_options.generation_iteration = 10000;
    m_options.target_type = T_SNAIL;
    m_options.target_file = "";
    m_options.visualizer = false;

    m_infos.state_selected = 0;
    m_infos.max_state_memory = 0;
    m_infos.number_steps = 0;
    m_infos.initalize = false;
    m_infos.length = 0;
    m_infos.size = 0;
}

// ----- Encapsulation ----- //

e_algorithm     Puzzle::getAlgorithm() const { return (m_options.algorithm); }
void            Puzzle::setAlgorithm(const e_algorithm& algorithm) { m_options.algorithm = algorithm; m_infos.initalize = false; }

e_heuristic     Puzzle::getHeuristic() const { return (m_options.heuristic); }
void            Puzzle::setHeuristic(const e_heuristic& heuristic) { m_options.heuristic = heuristic; m_infos.initalize = false; }

bool            Puzzle::isGenerate() const { return (m_options.generation); }
int             Puzzle::getGenerationLenght() const { return (m_options.generation_length); }
int             Puzzle::getGenerationIteration() const { return (m_options.generation_iteration); }
void            Puzzle::setGeneration(const bool& has_generation, const int& generation_length, const int& generation_iteration) { m_options.generation = has_generation; m_options.generation_length = generation_length; m_options.generation_iteration = generation_iteration; m_infos.initalize = false; }

e_target        Puzzle::getTargetType() const { return (m_options.target_type); }
std::string     Puzzle::getTargetFile() const { return (m_options.target_file); }
void            Puzzle::setTarget(const e_target& target_type, const std::string& target_file) { m_options.target_type = target_type; m_options.target_file = target_file; m_infos.initalize = false;}

bool            Puzzle::isVisualized() const { return (m_options.visualizer); }
void            Puzzle::setVisualizer(const bool& visualize) { m_options.visualizer = visualize; }

int             Puzzle::getNumberStateSelected() const { return (m_infos.state_selected); }

int             Puzzle::getMaxStateInMemory() const { return (m_infos.max_state_memory); }

int             Puzzle::getNumberSteps() const { return (m_infos.number_steps); }

std::list<State*>   Puzzle::getSteps() const { return (m_infos.steps); }

int             Puzzle::getLength() const { return (m_infos.length); }
int             Puzzle::getSize() const { return (m_infos.size); }

bool            Puzzle::isInitialized() const { return (m_infos.initalize); }

State           *Puzzle::getStart() const { return (m_start); }

State           *Puzzle::getTarget() const { return (m_target); }

std::set<State*, compare> Puzzle::getQueue() const { return (m_queue); }

std::unordered_map<size_t, State*> Puzzle::getList() const { return (m_list); }

// ----- Private Functions ----- //

int             Puzzle::launchVisualizer()
{
    Visualizer  v(m_infos);

    return (v.launch());
}