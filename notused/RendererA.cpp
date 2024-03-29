/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RendererA.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 17:14:18 by dpeck             #+#    #+#             */
/*   Updated: 2019/06/14 19:15:29 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RendererA.hpp"
#include "Quad.hpp"
#include "OpenGLHelper.hpp"
#include "Gameboard.hpp"
#include "SnakeSprite.hpp"
#include <vector>
#include <iomanip>
#include "sysconfig.hpp"

#include <unistd.h>
#include <stdio.h>

extern "C" IRenderer *create_renderer() {
    return new RendererA();
}

RendererA::RendererA() : 
	_borderOffset(Gameboard::squareSize * 2), _score(0), _pause(false), _obstaclesBuilt(false)
{
}

RendererA::RendererA(RendererA const & other)
{
    *this = other;
}

RendererA const & RendererA::operator=(RendererA const & rhs)
{
    if (this != &rhs)
    {
 	    this->_borderOffset = rhs._borderOffset;
	    this->_score = rhs._score;

        this->_pause = rhs._pause;
        this->_lost = rhs._lost;
        this->_pauseStr = rhs._pauseStr;

        this->_obstaclesBuilt = rhs._obstaclesBuilt;
        
        this->_appleX = rhs._appleX;
        this->_appleY = rhs._appleY;

        this->_ss.str("");
        this->_ss << rhs._ss.str();       
    }
    return (*this);
}

//everything in this class can't be created until GLFW init functions are called
//wanted to maintain the error checking of these functions. so use an init function rather than constructor
void RendererA::init()
{
    OpenGLDraw::resetObjects();

	buildBackground();
	buildBorder();
	buildApple();

    _ss.str("");
	_ss << std::setfill('0') << std::setw(3) << "";
	_pauseStr = "-> Continue <-\t   Quit";
    OpenGLInit::disable3dDepth();
}

bool RendererA::initGL()
{
	return (OpenGLHelper::initEverything());
}

void RendererA::cleanupGL()
{
	OpenGLHelper::cleanup();
}

RendererA::~RendererA()
{
}

void RendererA::refreshSnakeBuffer(std::vector<float> snakeVertices)
{
	OpenGLDraw::updateObjectDrawingInfo("snake", snakeVertices);
}

void RendererA::buildBackground()
{
	//could try tiling by the texture size instead of the squaresize. doesn't make much of a difference now. but might on a bigger window
    unsigned int totalRows = Gameboard::windowHeight / Gameboard::squareSize;
    unsigned int totalCols = Gameboard::windowWidth / Gameboard::squareSize;
    unsigned int posX = 0;
    unsigned int posY = 0;

    std::vector<float> quadCoords;
	std::vector<float> backgroundVertices;
    
    for (unsigned int i = 0; i < totalRows; i++)
    {
        posX = 0;
        for (unsigned int j = 0; j < totalCols; j++)
        {
            quadCoords = Quad::getPosCoords(posX, posY, Gameboard::squareSize);
            Quad::buildVertex(backgroundVertices, quadCoords, OpenGLDraw::getBufferFormat(0));
            posX += Gameboard::squareSize;
        }
        posY += Gameboard::squareSize;
    }

	OpenGLDraw::updateObjectDrawingInfo("background", backgroundVertices);
}

void RendererA::buildBorder()
{
	unsigned int borderSquare = Gameboard::squareSize * 2;
    unsigned int totalRows = (Gameboard::windowHeight - _borderOffset * 2) / borderSquare;
    unsigned int totalCols = (Gameboard::windowWidth - _borderOffset * 2) / borderSquare;
    unsigned int posX = _borderOffset;
    unsigned int botY = (totalRows) * borderSquare;

    std::vector<float> quadCoords;
	std::vector<float> borderVertices;
    
    for (unsigned int i = 0; i < totalCols; i++)
    {
        quadCoords = Quad::getPosCoords(posX, _borderOffset, borderSquare);
        Quad::buildVertex(borderVertices, quadCoords, OpenGLDraw::getBufferFormat(0));
        quadCoords = Quad::getPosCoords(posX, botY, borderSquare);
        Quad::buildVertex(borderVertices, quadCoords, OpenGLDraw::getBufferFormat(0));
        posX += borderSquare;
    }

    unsigned int rightX = (totalCols) * borderSquare;
    unsigned int posY = _borderOffset + borderSquare;

    for (unsigned int i = 1; i < totalRows; i++)
    {
        quadCoords = Quad::getPosCoords(_borderOffset, posY, borderSquare);
        Quad::buildVertex(borderVertices, quadCoords, OpenGLDraw::getBufferFormat(0));
        quadCoords = Quad::getPosCoords(rightX, posY, borderSquare);
        Quad::buildVertex(borderVertices, quadCoords, OpenGLDraw::getBufferFormat(0));
        posY += borderSquare;
    }

	OpenGLDraw::updateObjectDrawingInfo("border", borderVertices);
}

void RendererA::updateScore(unsigned int & score)
{
    _ss.str("");
    _ss << std::setfill('0') << std::setw(3) << score;
    _score = score;
}

void RendererA::draw()
{
	OpenGLDraw::clearScreen();

	float alpha = 1.0f;

	if (_pause || _lost)
		alpha = 0.3;
        
	OpenGLDraw::background(1.0f, 1.0f, 1.0f, 1.0 * alpha);
	OpenGLDraw::border(0.0f, 0.0f, 1.0f, 1.0f * alpha);
    OpenGLDraw::obstacles(1.0f, 0.0f, 1.0f, 1.0f * alpha);
	OpenGLDraw::score(_ss.str(), _borderOffset);
	OpenGLDraw::apple(_appleX, _appleY, 1.0f, 0.0f, 0.0f, 1.0f * alpha); // x and y value are unused in this version
	OpenGLDraw::snake(1.0f, 1.0f, 1.0f, 1.0f * alpha);

	if (_pause)
        OpenGLDraw::menu("Pause", _pauseStr);
    else if (_lost && _score < WIN_POINT)
		OpenGLDraw::menu("Continue ?", _pauseStr);
    else if (_lost)
        OpenGLDraw::menu("You win !", _pauseStr);
	OpenGLDraw::swapBuffers();
}

void RendererA::buildApple()
{
    float apple[] = {
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

	std::vector<float> appleVertices(apple, apple + sizeof(apple) / sizeof(float));
	OpenGLDraw::updateObjectDrawingInfo("appleBuild", appleVertices);
}

void RendererA::updateApple(const float & x, const float & y)
{
	this->_appleX = x;
	this->_appleY = y;
}

void RendererA::processInput(Direction & curDirection, unsigned int & renderer)
{
    if (_lost)
    {
        OpenGLInput::menuInput(curDirection, _pauseStr, true);
		
		if (curDirection == Restart)
			_lost = false;
    }
	else if (_pause == false)
	{	
		OpenGLInput::gameInput(curDirection);

		if (curDirection == Pause)
			_pause = true;
	}
	else
	{
		OpenGLInput::menuInput(curDirection, _pauseStr, false);
		
		if (curDirection == Pause)
			_pause = false;
	}

    OpenGLInput::rendererSelection(renderer);
}

void RendererA::buildSnakeVertex(float x, float y, std::deque<float> & buffer, std::string texture)
{
    static_cast<void>(texture);
    std::vector<float> startingPositions;
	startingPositions = Quad::getPosCoords(x, y, Gameboard::squareSize);
    Quad::buildVertex(buffer, startingPositions, OpenGLDraw::getBufferFormat(0));	
}

void RendererA::changeSnakeTexture(bool tail, unsigned int size, std::deque<float> & buffer, std::string texture)
{
    static_cast<void>(tail);
    static_cast<void>(size);
    static_cast<void>(buffer);
    static_cast<void>(texture);
    return;
}

void RendererA::popSnakeTail(std::deque<float> & buffer)
{
	for (unsigned int i = 0; i < Quad::_rows; i++)
	{
		for (unsigned int j = 0; j < Quad::_offset; j++)
			buffer.pop_front();
    }
}

bool RendererA::obstaclesHaveBeenBuilt()
{
    return (_obstaclesBuilt);
}

void RendererA::buildObstacles(std::vector<float> x, std::vector<float> y)
{
    std::vector<float> obstacleCoords;

    for (unsigned int i = 0; i < x.size(); i++)
    {
        std::vector<float> obstaclePos = Quad::getPosCoords(x[i], y[i], Gameboard::squareSize);
        Quad::buildVertex(obstacleCoords, obstaclePos, OpenGLDraw::getBufferFormat(0));
    }
    OpenGLDraw::updateObjectDrawingInfo("obstacles", obstacleCoords);
    _obstaclesBuilt = true;
}

void RendererA::setLost(bool val) {
        _lost = val;
        if (val == true)
            _pauseStr = "-> Restart <-\t   Quit"; }

void RendererA::setPause(bool val)
{
    _pause = val;
}