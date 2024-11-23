#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

typedef unsigned int	uint;
typedef unsigned char	bytE;

struct ffRGBA
{
	bytE m_r;
	bytE m_g;
	bytE m_b;
	bytE m_a;

	ffRGBA(bytE _r = 255,
		bytE _g = 255,
		bytE _b = 255,
		bytE _a = 255)
	{
		m_r = _r;
		m_g = _g;
		m_b = _b;
		m_a = _a;
	}
};

template<typename T>
struct tVec3
{
	T	m_x;
	T	m_y;
	T	m_z;
	tVec3(T _x = 0, T _y = 0, T _z = 0)
	{
		m_x = _x;
		m_y = _y;
		m_z = _z;
	}
};

template<typename T>
struct tVec2
{
	T	m_x;
	T	m_y;

	tVec2(T _x = 0, T _y = 0)
	{
		m_x = _x;
		m_y = _y;
	}
};