#include <fstream>
#include <sstream>
#include "RecommenderSystemLoader.h"

void
parse_all_movies (RecommenderSystem &rs, std::stringstream &movies_file,
                  std::string &movie_name,
                  int movie_year, std::string &line);

std::stringstream
get_system_stream (const std::string &movies_file_path);

std::unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file
(const std::string &movies_file_path) noexcept (false)
{
  RecommenderSystem rs;
  std::stringstream movies_file = get_system_stream (movies_file_path);
  std::string movie_name;
  int movie_year = 0;
  std::string features;
  std::string line;
  parse_all_movies (rs, movies_file, movie_name, movie_year, line);
  return std::move(std::make_unique<RecommenderSystem>(rs));
}

void
parse_all_movies (RecommenderSystem &rs, std::stringstream &movies_file,
                                           std::string &movie_name,
                                           int movie_year, std::string &line)
{
  while (std::getline(movies_file, line))
  {
    std::stringstream ss(line);
    std::getline (ss, movie_name, NAME_YEAR_DELIMITER);
    ss >> movie_year;
    double entry;
    std::vector<double> feat_vec;
    while (ss >> entry)
    {
      if (entry < MIN_VAL || entry > MAX_VAL)
      {
        throw std::runtime_error (ERR_MSG_INVALID_FEAT_VAL);
      }
      feat_vec.push_back (entry);
    }
    rs.add_movie (movie_name, movie_year, feat_vec);
  }
}

std::stringstream
get_system_stream (const std::string &movies_file_path)
{
  std::ifstream is(movies_file_path);
  if (!is)
  {
    throw std::runtime_error(ERR_MSG_OPEN_FILE);
  }
  if (is)
  {
    is.seekg (0, std::ifstream::end);
    long length = is.tellg();
    is.seekg (0, std::ifstream::beg);
    char *buffer = new char [length];
    is.read (buffer, length);
    if (is)
    {
      std::stringstream movies_file((std::string(buffer)));
      delete[] buffer;
      return movies_file;
    }
    else
    {
      delete[] buffer;
      throw std::runtime_error(ERR_MSG_READ_FILE);
    }
  }
  std::stringstream movies_file;
  return movies_file;
}


