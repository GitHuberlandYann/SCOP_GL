#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include "scop.h"

class InvalidFileException : public std::exception
{
	public:
		const char *what() const throw();
};

class InvalidMltFileException : public std::exception
{
	public:
		const char *what() const throw();
};

class InvalidVertexException : public std::exception
{
	public:
		const char *what() const throw();
};

class InvalidFaceException : public std::exception
{
	public:
		const char *what() const throw();
};

class DoubleMltlibException : public std::exception
{
	public:
		const char *what() const throw();
};

class NoMltlibFileException : public std::exception
{
	public:
		const char *what() const throw();
};

class MltExtensionException : public std::exception
{
	public:
		const char *what() const throw();
};

class InvalidMltException : public std::exception
{
	public:
		const char *what() const throw();
};

class NoMatchingMaterialException : public std::exception
{
	public:
		const char *what() const throw();
};

class InvalidRGBException : public std::exception
{
	public:
		const char *what() const throw();
};

class EmptyObjectException : public std::exception
{
	public:
		const char *what() const throw();
};

#endif
