#include "Exception.hpp"

const char* InvalidFileException::what() const throw()
{
	return ("[InvalidFileException] Input file could not be opened.");
}

const char* InvalidMltFileException::what() const throw()
{
	return ("[InvalidMltFileException] Mlt file could not be opened.");
}

const char* InvalidVertexException::what() const throw()
{
	return ("[InvalidVertexException] Vertex line (v, vt, vn, kd) is invalid.");
}

const char* InvalidFaceException::what() const throw()
{
	return ("[InvalidFaceException] Face line is invalid.");
}

const char* DoubleMltlibException::what() const throw()
{
	return ("[DoubleMltlibException] Two or more lines begin with mltlib.");
}

const char* NoMltlibFileException::what() const throw()
{
	return ("[NoMltlibFileException] mltlib line is missing a file.");
}

const char* MltExtensionException::what() const throw()
{
	return ("[MltExtensionException] mltlib must be .mlt file.");
}

const char* InvalidMltException::what() const throw()
{
	return ("[InvalidMltException] Error in .mlt file.");
}

const char* MltlibNoUseException::what() const throw()
{
	return ("[MltlibNoUseException] Mltlib line without usemlt line afterwards.");
}

const char* MltlibNoStartException::what() const throw()
{
	return ("[MltlibNoStartException] Mltlib line after v line.");
}

const char* NoMatchingMaterialException::what() const throw()
{
	return ("[NoMatchingMaterialException] No matching material in .mlt file.");
}

const char* InvalidRGBException::what() const throw()
{
	return ("[InvalidRGBException] RGB in .mlt must be in range [0:1].");
}

const char* SOILFailureException::what() const throw()
{
	return ("[SOILFailureException]");
}

const char* EmptyObjectException::what() const throw()
{
	return ("[EmptyObjectException] .obj has no faces.");
}
