#ifndef BADOOLIZATION_INPUTSTATE_H_INCLUDED
#define BADOOLIZATION_INPUTSTATE_H_INCLUDED

#include <map>
#include <iostream>
#include <string>

namespace badoolization {

    struct InputState {
        
        // map of resources comforming this state
        std::map<std::string,int> resources;

        bool read_from(std::istream& stream);
		void log(std::ostream& stream, const char* name) const;
    };

} // namespace badoolization

#endif // BADOOLIZATION_INPUTSTATE_H_INCLUDED
