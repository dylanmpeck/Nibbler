/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RendererA.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 17:11:13 by dpeck             #+#    #+#             */
/*   Updated: 2019/06/14 19:15:35 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERERA_HPP
#define RENDERERA_HPP

#include "IRenderer.hpp"
#include <string>
#include <sstream>

//This is the basic 2d renderer. Only handles position coordinates.

class RendererA : public IRenderer {

	unsigned int _borderOffset;
	unsigned int _score;

	bool _pause;
	bool _lost;
	std::string _pauseStr;

	bool _obstaclesBuilt;

	float _appleX;
	float _appleY;

    std::ostringstream _ss;
	
public:
	RendererA();
	~RendererA();
	RendererA(RendererA const &);
	RendererA const & operator=(RendererA const &);

    void init();
	bool initGL();

    void buildBorder();
	void buildBackground();
    void buildApple();

	bool obstaclesHaveBeenBuilt();
	void buildObstacles(std::vector<float> x, std::vector<float> y);
	
	void refreshSnakeBuffer(std::vector<float> snakeBuffer);
	void updateScore(unsigned int & score);
	void updateApple(const float & x, const float & y);

    void popSnakeTail(std::deque<float> & buffer);
	void buildSnakeVertex(float x, float y, std::deque<float> & buffer, std::string texture);
	void changeSnakeTexture(bool tail, unsigned int size, std::deque<float> & buffer, std::string texture);

    void processInput(Direction & curDirection, unsigned int & renderer);

	void draw();

	void cleanupGL();
	void setLost(bool);
	void setPause(bool);
};

#endif