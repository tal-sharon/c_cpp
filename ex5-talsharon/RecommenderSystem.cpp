#include <cmath>
#include <algorithm>
#include "RecommenderSystem.h"

sp_movie
RecommenderSystem::add_movie (const std::string &name, int year,
                              const features_vec &features)
{
  _feat_vec_len = features.size();
  Movie m(name, year);
  sp_movie p = std::make_shared<Movie>(m);
  _movies[p] = features;
  return p;
}

sp_movie RecommenderSystem::recommend_by_content (const RSUser &user)
{
  double ranks_avg = get_user_ranks_avg (user);
  features_vec pref_vec(_feat_vec_len, 0);
  double pref_norm = fill_pref_vec (user, ranks_avg, pref_vec);
  return get_similar_by_content (user, pref_vec, pref_norm);
}


sp_movie RecommenderSystem::get_similar_by_content (const RSUser &user,
                                                    const features_vec &vec,
                                                    double norm)
{
  sp_movie recommend_movie = nullptr;
  double match_rate = NEGATIVE_RATE;
  for (const auto &movie: _movies)
  {
    if (user.get_ranks ().count (movie.first) == 0)
    {
      double numerator = 0;
      double cur_norm = 0;
      for (size_t i = 0; i < _feat_vec_len; i++)
      {
        numerator += movie.second[i] * vec[i];
        cur_norm += movie.second[i] * movie.second[i];
      }
      cur_norm = sqrt (cur_norm);
      double cur_match = (numerator / (norm * cur_norm));
      if (match_rate < cur_match)
      {
        recommend_movie = movie.first;
        match_rate = cur_match;
      }
    }
  }
  return recommend_movie;
}

double RecommenderSystem::fill_pref_vec (const RSUser &user, double ranks_avg,
                                   features_vec &pref_vec)
{
  double pref_norm = 0;
  for (const auto& movie : user.get_ranks())
  {
    int i = 0;
    for (auto& entry : pref_vec)
    {
      entry += _movies[movie.first][i] * (movie.second - ranks_avg);
      pref_norm += entry*entry;
      i++;
    }
  }
  pref_norm = sqrt(pref_norm);
  return pref_norm;
}

double RecommenderSystem::get_user_ranks_avg (const RSUser &user) const
  {
    double ranks_sum = 0;
    unsigned int ranks_num = 0;
    for (const auto& movie : user.get_ranks())
    {
      ranks_sum += movie.second;
      ranks_num++;
    }
    double ranks_avg = ranks_sum / ranks_num;
    return ranks_avg;
  }

double RecommenderSystem::calc_norm(const features_vec& vec)
{
  double norm = 0;
  for (const auto& entry : vec)
  {
    norm += entry*entry;
  }
  return sqrt(norm);
}


sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  sp_movie recommended_movie = nullptr;
  double max_rate = 0;

  movie_maps_pair movie_maps = make_movie_maps (user);
  std::map<sp_movie, features_vec> not_watched = movie_maps.first;
  std::map<sp_movie, features_vec> watched = movie_maps.second;

  for (const auto& not_watched_movie : not_watched)
  {
    double predicted_rate = predict_rate (user, k, watched, not_watched_movie);
    if (max_rate < predicted_rate)
    {
      max_rate = predicted_rate;
      recommended_movie = not_watched_movie.first;
    }
  }
  return recommended_movie;
}

double
RecommenderSystem::predict_rate (const RSUser &user, int k, std::map<sp_movie,
                                 features_vec> &watched,
                                 const std::pair<const sp_movie,
                                 features_vec> &not_watched_movie)
{
  double norm = calc_norm (not_watched_movie.second);
  rates_vec rates;
  get_similarities (watched, not_watched_movie, norm, rates);
  std::sort (rates.begin(), rates.end(), cmp_movie_by_similarity);
  double predicted_rate = calc_prediction (user, k, rates);
  return predicted_rate;
}

double
RecommenderSystem::calc_prediction (const RSUser &user, int k, const
rates_vec &rates) const
{
  double numerator = 0;
  double denominator = 0;
  auto& ranks = const_cast<rank_map &>(user.get_ranks ());
  for (int i = 0; i < k; i++)
  {
    std::pair<sp_movie, double> cur_pair = rates[i];
    numerator += ranks[cur_pair.first] * cur_pair.second;
    denominator += cur_pair.second;
  }
  return numerator/denominator;
}

void
RecommenderSystem::get_similarities (std::map<sp_movie, features_vec> &watched,
                                     const std::pair<const sp_movie,
                                     features_vec> &not_watched_movie,
                                     double norm, rates_vec &rates) const
{
  for (const auto& watched_movie : watched)
  {
    double numerator = 0;
    double cur_norm = 0;
    for (size_t i = 0; i < _feat_vec_len; i++)
    {
      numerator += watched_movie.second[i] * not_watched_movie.second[i];
      cur_norm += watched_movie.second[i] * watched_movie.second[i];
    }
    cur_norm = sqrt (cur_norm);
    double cur_match = (numerator / (norm * cur_norm));
    rates.emplace_back(watched_movie.first, cur_match);
  }
}

movie_maps_pair
RecommenderSystem::make_movie_maps (const RSUser &user)
{
  std::map<sp_movie, features_vec> not_watched;
  std::map<sp_movie, features_vec> watched;
  for (const auto& movie : _movies)
  {
    if (user.get_ranks().count (movie.first) == 0)
    {
      not_watched[movie.first] = movie.second;
    } else {
      watched[movie.first] = movie.second;
    }
  }
  return std::make_pair (not_watched, watched);
}

double
RecommenderSystem::predict_movie_score (const RSUser &user,
                                        const sp_movie &movie, int k)
{
  movie_maps_pair movie_maps = make_movie_maps (user);
  std::map<sp_movie, features_vec> not_watched = movie_maps.first;
  std::map<sp_movie, features_vec> watched = movie_maps.second;
  features_vec vec = _movies[movie];
  std::pair<sp_movie, features_vec> not_watched_movie = std::make_pair
      (movie, vec);
  return predict_rate (user, k, watched, not_watched_movie);
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  sp_movie movie = std::make_shared<Movie>(Movie(name, year));
  auto found = _movies.find (movie);
  if (found != _movies.end())
  {
    return found->first;
  }
  return nullptr;
}

std::ostream &operator<< (std::ostream &os, const RecommenderSystem &rs)
{
  for (const auto& movie : rs._movies)
  {
    os << *movie.first << std::endl;
  }
  return os;
}
