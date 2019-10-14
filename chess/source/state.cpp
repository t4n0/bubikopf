#include <chess/state.h>

namespace Chess {

Piece State::GetPieceFrom(const File file, const Rank rank) const {
  const CodedRank& codes{board_.at(RankMap.at(rank))};
  const int column{MapToColumn[file]};

  int column_counter{0};
  for (const Code& code : codes) {
    column_counter += GetCoveredColumns(code);
    if (column_counter >= column) {
      return GetPiece(code);
    }
  }
  throw std::runtime_error{"GetPieceFrom failed."};
}

namespace {

struct AffectedCode {
  std::size_t idx{};
  // the file covered by foregoing code (i.e. the one left of idx)
  int foregoing_file{};
};

AffectedCode GetAffectedCode(const CodedRank& rank, const int column) {
  int column_counter{0};
  int column_counter_last{0};
  std::size_t idx{0};
  for (const Code& square : rank) {
    column_counter += GetCoveredColumns(square);
    if (column_counter >= column) {
      return AffectedCode{idx, column_counter_last};
    }
    idx++;
    column_counter_last = column_counter;
  }

  throw std::runtime_error{"GetAffectedCode failed."};
}

CodedRank EncodePiece(const Code code_prior, const int foregoing_column,
                          const Piece piece, const int column) {
  CodedRank codes_posterior{};
  const int column_diff_foregoing = column - foregoing_column - 1;
  const int column_diff_following =
      foregoing_column + GetCoveredColumns(code_prior) - column;

  if (column_diff_foregoing) {
    codes_posterior.emplace_back(column_diff_foregoing);
  }
  codes_posterior.emplace_back(piece);
  if (column_diff_following) {
    codes_posterior.emplace_back(column_diff_following);
  }
  return codes_posterior;
}

CodedRank MergeCodes(const CodedRank& codes_prior,
                         const std::size_t idx_insertion,
                         const CodedRank& codes_insertion) {
  CodedRank codes_posterior{};

  std::copy(begin(codes_prior),
            begin(codes_prior) + static_cast<int>(idx_insertion),
            std::back_inserter(codes_posterior));
  std::copy(begin(codes_insertion), end(codes_insertion),
            std::back_inserter(codes_posterior));
  std::copy(begin(codes_prior) + +static_cast<int>(idx_insertion) + 1,
            end(codes_prior), std::back_inserter(codes_posterior));

  CodedRank::iterator it_collector{nullptr};
  bool collecting{0};
  for (CodedRank::iterator it{begin(codes_posterior)};
       it != end(codes_posterior); ++it) {
    bool piece_decoded{IsAPiece(*it)};
    if (!collecting && !piece_decoded) {
      collecting = true;
      it_collector = it;
    } else if (collecting && !piece_decoded) {
      *it_collector = GetEmptySquares(*it_collector) + GetEmptySquares(*it);
    } else if (collecting && piece_decoded) {
      collecting = false;
    }
  }

  bool foregoing_is_empty{false};
  CodedRank::iterator new_end{
      std::remove_if(begin(codes_posterior), end(codes_posterior),
                     [&foregoing_is_empty](const Code code) {
                       bool piece_decoded{IsAPiece(code)};
                       if (piece_decoded) {
                         foregoing_is_empty = false;
                         return false;
                       } else if (!piece_decoded && !foregoing_is_empty) {
                         foregoing_is_empty = true;
                         return false;
                       } else {
                         return true;
                       }
                     })};

  codes_posterior.erase(new_end, end(codes_posterior));

  return codes_posterior;
}

}  // namespace

void State::SetSquareTo(const File file, const Rank rank, const Piece piece) {
  CodedRank& codes{board_.at(RankMap.at(rank))};
  const int column{MapToColumn[file]};

  const AffectedCode affected_code{GetAffectedCode(codes, column)};
  const CodedRank codes_insertion{EncodePiece(codes.at(affected_code.idx),
                                                  affected_code.foregoing_file,
                                                  piece, column)};
  CodedRank codes_posterior{
      MergeCodes(codes, affected_code.idx, codes_insertion)};

  std::swap(codes, codes_posterior);
}

std::ostream& operator<<(std::ostream& stream, const State& state) {
  for (const Rank rank : RANKS) {
    for (const File file : FILES) {
      stream << state.GetPieceFrom(file, rank) << ' ';
    }
    stream << '\n';
  }
  return stream;
}

}  // namespace Chess
