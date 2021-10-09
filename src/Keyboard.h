//
// Created by stefan on 8/21/21.
//

#ifndef OVERTONE_KEYBOARD_H
#define OVERTONE_KEYBOARD_H


#include <initializer_list>
#include "Spectrum.h"
#include <memory>

/**
 * This class projects the audio spectra onto the 88 keys of the keyboard.
 */
class Keyboard
{
public:
    using Vector = std::vector<double>;
    using KeyRange = std::pair<unsigned char, unsigned char>;

    /**
     * constructor that evaluates `keyboard` for the first frame of the video
     * @param spectra audio spectra of the keyboard sections
     */
    Keyboard(std::initializer_list<Spectrum> spectra);

    std::shared_ptr<Vector> get_keyboard() const
    {
        return keyboard;
    }

    /**
     * Evaluates `keyboard` for the next video frame if there is a next video
     * frame.
     * @return false if there is no next video frame
     */
    bool go_to_next_frame();
private:
    // audio spectra of the keyboard sections
    std::vector<Spectrum> spectra;

    // vector that contains the signal of each key of the keyboard
    std::shared_ptr<Vector> keyboard;

    /**
     * Evaluates `keyboard`.
     */
    void evaluate_keys();

    /**
     * evaluate_keys() evaluates the weighted average of all the signals that
     * belong to a certain key on the keyboard. This function determines the
     * weight of such a signal.
     * @param key position of a certain frequency on the keyboard, e.g., 440 Hz
     *            would be at 48.0
     * @param assigned_key index of the key on the keyboard to which the signal
     *                     belongs, i.e., an integer from 0 to 87
     * @return weight
     */
    inline static double evaluate_weight(const double &key,
                                         const unsigned char &assigned_key);
};


#endif //OVERTONE_KEYBOARD_H
