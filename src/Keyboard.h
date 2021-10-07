//
// Created by stefan on 8/21/21.
//

#ifndef OVERTONE_KEYBOARD_H
#define OVERTONE_KEYBOARD_H


#include <initializer_list>
#include "Spectrum.h"
#include <memory>

class Keyboard
{
public:
    using Vector = std::vector<double>;
    using KeyRange = std::pair<unsigned char, unsigned char>;
    Keyboard(std::initializer_list<Spectrum> spectra);
    std::shared_ptr<Vector> get_keyboard() const
    {
        return keyboard;
    }
    bool go_to_next_frame();
private:
    std::vector<Spectrum> spectra;
    std::shared_ptr<Vector> keyboard;
    void evaluate_keys();
    inline static double evaluate_weight(const double &key,
                                         const unsigned char &assigned_key);
};


#endif //OVERTONE_KEYBOARD_H
