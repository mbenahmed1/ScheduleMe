#pragma once

#ifndef UOSRCP_IO_H
#define UOSRCP_IO_H

#include "instance.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <iterator>

namespace ScheduleMe {

/**
 * Exception indicating I/O errors.
 */
struct Io_error : public std::runtime_error {
    Io_error(const std::string& msg)
      : std::runtime_error(msg)
    {
    }
};


/**
 * Read an instance from the given input stream.
 *
 * \return  instance on success
 * \throws  Io_error.
 */
Instance read_instance(std::istream& is);


/**
 * Read an instance from the given file path.
 *
 * \return  instance on success
 * \throws  Io_error.
 */
Instance read_instance(const std::string& path);


/**
 * Read a solution from the given path.
 *
 * \return List of start times index by activity indices.
 * \throws Io_error.
 */
std::vector<unsigned int> read_solution(const std::string& path);


/**
 * Read a solution from the given input stream.
 *
 * \return List of start times index by activity indices.
 * \throws Io_error.
 */
std::vector<unsigned int> read_solution(std::istream& is);



/**
 * Write a solution to the given stream.
 *
 * \param begin  iterator begin
 * \param end  iterator end
 * \param os  output stream
 */
template <typename InputIt>
void write_solution(InputIt begin, InputIt end, std::ostream& os)
{
    const auto n = std::distance(begin, end);

    os << n << std::endl;

    for (unsigned int j = 1; begin != end; ++begin, ++j) {
        os << j << ' ' << *begin << std::endl;
    }
}


/**
 * Write a solution to the given file.
 *
 * \param begin  iterator begin
 * \param end  iterator end
 * \param filename  file to write solution into
 */
template <typename InputIt>
void write_solution(InputIt begin, InputIt end, const std::string& filename)
{
    auto os = std::ofstream(filename);
    write_solution(begin, end, os);
}


/**
 * Write a solution to the given stream.
 *
 * \param container  container supporting std::begin, std::end
 * \param os  output stream
 */
template <typename Container>
void write_solution(const Container& solution, std::ostream& os)
{
    write_solution(std::begin(solution), std::end(solution), os);
}


/**
 * Write a solution to the given file.
 *
 * \param container  container supporting std::begin, std::end
 * \param filename  file to write solution into
 */
template <typename Container>
void write_solution(const Container& solution, const std::string& filename)
{
    auto os = std::ofstream(filename);
    write_solution(std::begin(solution), std::end(solution), os);
}


} // namespace uosrcp

#endif
