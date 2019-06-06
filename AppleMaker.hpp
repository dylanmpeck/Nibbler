/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AppleMaker.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpeck <dpeck@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/01 22:03:03 by dpeck             #+#    #+#             */
/*   Updated: 2019/06/01 22:16:03 by dpeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPLEMAKER_HPP
#define APPLEMAKER_HPP

#include <random>

class AppleMaker {

private:
    const unsigned int &_width;
    const unsigned int &_height;
    const unsigned int _squareSize;
    const unsigned int _rows;
    const unsigned int _cols;
    std::pair<float, float> _pos;

    std::random_device rd;
    std::mt19937 mt;
    std::uniform_int_distribution<int> rowRange;
    std::uniform_int_distribution<int> colRange;

public:
    AppleMaker(unsigned int & width, unsigned int & height, unsigned int squareSize);
    ~AppleMaker();

    void generateRandomPos();
    float getX();
    float getY();

};

#endif