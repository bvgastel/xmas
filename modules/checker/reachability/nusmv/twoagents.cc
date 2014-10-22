
#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

template <typename T>
string NumberToString(T Number) {
  ostringstream ss;
  ss << Number;
  return ss.str();
}

class QueueVar {
 public:
  QueueVar() = default;
  QueueVar(string &&name, vector<string> &&type, int capacity)
      : name_(name), type_(type), capacity_(capacity) {}

  const string &name() const { return name_; }
  string type() const { 
    string res = "{";
    bool first = true;

    if (none()) {
      res += "none";
      first = false;
    }
    for (auto &t : type_) {
      res += (first ? "" : ", ") + t;
      first = false;
    }

    return res + "}";
  }

  bool none() const { return type_.size() != 1; /*true;*/ }
  bool constant() const { return !none(); }
  string size_name() const { return name_ + "n"; }
  int capacity() const { return capacity_; }
  string front() const { return at(capacity_); }

  string decl() const {
    return name_ + ": array 1.." + NumberToString(capacity_) + " of " + type() +
           ";";
  }

  string at(int i) const { return name_ + "[" + NumberToString(i) + "]"; }

  string size_decl() const {
    return size_name() + ": 0.." + NumberToString(capacity_) + ";";
  }

 private:
  string name_;
  vector<string> type_;
  int capacity_;
};

class Trans {
 public:
  // Trans(string &&name, vector<QueueVar *> &&out, vector<QueueVar *> &&in,
  //       vector<string> &&var_in)
  //     : Trans(std::move(name), std::move(out), std::move(in), std::move(var_in),
  //             vector<string>{}) {}
  Trans(string &&name, vector<QueueVar *> &&out, vector<QueueVar *> &&in,
        vector<string> &&var_in, vector<string> &&cond)
      : name_(name),
        in_(std::move(in)),
        out_(std::move(out)),
        var_in_(std::move(var_in)),
        cond_(std::move(cond)) {
    assert(is_sorted(in_.begin(), in_.end()));
    assert(is_sorted(out_.begin(), out_.end()));
    assert(var_in_.size() == in_.size());
  }
  const string &name() const { return name_; }
  const vector<QueueVar *> &in() const { return in_; }
  const vector<string> &var_in() const { return var_in_; }
  const vector<QueueVar *> &out() const { return out_; }
  const vector<string> &cond() const { return cond_; }

 private:
  string name_;
  vector<QueueVar *> in_, out_;
  vector<string> var_in_, cond_;
};

class Q {
 public:
  Q(string &&name, int length) : name_(move(name)), length_(length) {}
  const string& name() const { return name_; }
  int length() const { return length_; }

 private:
  string name_;
  int length_;
};

namespace std {
ostream &operator<<(ostream &out, const Q &q) {
  out << q.name();
  return out;
}

ostream &operator<<(ostream &out, const Q * const q) {
  out << q->name();
  return out;
}

ostream &operator<<(ostream &out, const QueueVar &q) {
  out << q.name();
  return out;
}

ostream &operator<<(ostream &out, const QueueVar * const q) {
  out << q->name();
  return out;
}

ostream &operator<<(ostream &out, const Trans &q) {
  out << q.name();
  return out;
}

ostream &operator<<(ostream &out, const Trans * const q) {
  out << q->name();
  return out;
}
}

int main(int argc, char *argv[]) {
  vector<string> params{argv, argv+argc};

  bool use_trans = false;
  bool trace = false;
  for (int i = 1; i < argc; i++) {
    // use INIT/TRANS
    if (params[i] == "-t") {
      use_trans = true;
    }

    // use ASSIGN
    if (params[i] == "-a") {
      use_trans = false;
    }

    if (params[i] == "-tt") {
      use_trans = true;
      trace = true;
    }
  }

  cout << "MODULE main" << endl;
  cout << "VAR" << endl;

  const auto k = 4;
  const auto num_cq = 4;
  const auto num_iq = 4;
  
  const auto k_x = 2;
  const auto num_cx = 4;
  const auto num_dx = 2;
  
  const auto k_cc = 4;
  const auto num_cc = 4;

#define TOTAL_QUEUES (num_cq + num_iq + num_cx + num_dx + num_cc)

  array<QueueVar, TOTAL_QUEUES> queues;
  using S = vector<string>;

  int i = 0;
  auto cc = queues.begin() + i - 1;
  for (int j = 1; j <= num_cc; ++j) {
    queues[i] = QueueVar("cc" + NumberToString(j) + "_q", S{"token"}, k_cc);
    i++;
  }

  auto cq = queues.begin() + i - 1;
  for (int j = 1; j <= num_cq; ++j) {
    queues[i] = QueueVar("cq" + NumberToString(j), S{"token"}, k);
    i++;
  }

  auto cx = queues.begin() + i - 1;
  for (int j = 1; j <= num_cx; ++j) {
    queues[i] = QueueVar("cx" + NumberToString(j), S{"token"}, k_x);
    i++;
  }

  auto iq = queues.begin() + i - 1;
  queues[i++] = QueueVar("iq1", S{"req"}, k);
  queues[i++] = QueueVar("iq2", S{"rsp"}, k);
  queues[i++] = QueueVar("iq3", S{"rsp"}, k);
  queues[i++] = QueueVar("iq4", S{"req"}, k);
  
  auto dx = queues.begin() + i - 1;
  for (int j = 1; j <= num_dx; ++j) {
    queues[i] = QueueVar("dx" + NumberToString(j), S{"rsp", "req"}, k_x);
    i++;
  }

  vector<QueueVar *> sorted_queues;
  for (auto &q : queues) {
    sorted_queues.push_back(&q);
  }

  assert(is_sorted(sorted_queues.begin(), sorted_queues.end()));

  auto t = "T";
  auto num_t = 18;
  /* name, out, in, conditions */
  using QS = vector<decltype(cc)>;
  auto ts = vector<Trans>{};

  // ts.emplace_back("idle", QS{}, QS{}, S{}, S{});

  ts.emplace_back("T1", QS{&cq[1]},
                        QS{&dx[1]}, S{"req"},
                        S{});

  ts.emplace_back("T2", QS{&dx[1]}, 
                        QS{&iq[1]}, S{dx[1].front()},
                        S{dx[1].front() + " = req"});

  ts.emplace_back("T3", QS{&dx[1]}, 
                        QS{&iq[2]}, S{dx[1].front()},
                        S{dx[1].front() + " = rsp"});

  ts.emplace_back("T4", QS{&cc[2], &iq[2]}, 
                        QS{}, S{},
                        S{});

  ts.emplace_back("T5", QS{}, 
                        QS{&cc[2], &cx[2]}, S{"token", "token"},
                        S{});

  ts.emplace_back("T6", QS{&cx[2]}, 
                        QS{&cq[2]}, S{cx[2].front()},
                        S{});

  ts.emplace_back("T7", QS{&cc[1], &cq[3], &iq[1]}, 
                        QS{&dx[2]}, S{"rsp"},
                        S{});

  ts.emplace_back("T8", QS{}, 
                        QS{&cc[1], &cx[1]}, S{"token", "token"},
                        S{});

  ts.emplace_back("T9", QS{&cx[1]},
                        QS{&cq[1]}, S{cx[1].front()},
                        S{});

  ts.emplace_back("T10", QS{&dx[2]}, 
                         QS{&iq[4]}, S{dx[2].front()},
                         S{dx[2].front() + " = req"});

  ts.emplace_back("T11", QS{&dx[2]},
                         QS{&iq[3]}, S{dx[2].front()},
                         S{dx[2].front() + " = rsp"});

  ts.emplace_back("T12", QS{&cq[4]}, 
                         QS{&dx[2]}, S{"req"},
                         S{});

  ts.emplace_back("T13", QS{&cx[4]}, 
                         QS{&cq[4]}, S{cx[4].front()},
                         S{});

  ts.emplace_back("T14", QS{}, 
                         QS{&cc[4], &cx[4]}, S{"token", "token"},
                         S{});

  ts.emplace_back("T15", QS{&cx[3]}, 
                         QS{&cq[3]}, S{cx[3].front()},
                         S{});

  ts.emplace_back("T16", QS{},
                         QS{&cc[3], &cx[3]}, S{"token", "token"},
                         S{});

  ts.emplace_back("T17", QS{&cc[3], &iq[3]}, 
                         QS{}, S{},
                         S{});

  ts.emplace_back("T18", QS{&cc[4], &cq[2], &iq[4]}, 
                         QS{&dx[1]}, S{"rsp"},
                         S{});

  for (auto& q : queues) {
    cout << "  " << q.decl() << endl;
    cout << "  " << q.size_decl() << endl;
  }

  if (!use_trans || trace) {
    cout << "  t: {";
    bool first = true;
    for (auto &t : ts) {
      cout << (first ? "" : ", ") << t;
      first = false;
    }
    cout << "};" << endl << endl;
  }

  if (!use_trans) {
    for (auto &t : ts) {
      for (auto &out : t.out()) {
        cout << "INVAR t = " << t << " -> " << out->size_name() << " != 0;" << endl;
      }

      for (auto &in : t.in()) {
        cout << "INVAR t = " << t << " -> " << in->size_name()
             << " != " << in->capacity() << ";" << endl;
      }

      for (auto &cond : t.cond()) {
        cout << "INVAR t = " << t << " -> " << cond << ";" << endl;
      }

      cout << endl;
    }
  }

  if (!use_trans) {
    cout << "ASSIGN" << endl;

    for (auto &q : queues) {
      cout << "  init(" << q.size_name() << ") := 0;" << endl;
    }

    cout << endl;

    for (auto &q : queues) {
      cout << "  next(" << q.size_name() << ") :=" << endl;
      cout << "    case" << endl;
      for (auto &t : ts) {
        for (auto &out : t.out()) {
          if (&q == out) {
            cout << "      (t = " << t << " & " << q.size_name()
                 << " > 0): " << q.size_name() << " - 1;" << endl;
          }
        }

        for (auto &in : t.in()) {
          if (&q == in) {
            cout << "      (t = " << t << " & " << q.size_name() << " < "
                 << q.capacity() << "): " << q.size_name() << " + 1;" << endl;
          }
        }
      }
      cout << "      TRUE: " << q.size_name() << ";" << endl;
      cout << "    esac;" << endl << endl;
    }

    for (auto &q : queues) {
      if (q.constant()) continue;
      for (int i = 1; i <= q.capacity(); ++i) {
        cout << "  init(" << q.at(i) << ") := none;" << endl;
        cout << "  next(" << q.at(i) << ") :=" << endl;
        cout << "    case" << endl;

        for (auto &t : ts) {
          for (auto &out : t.out()) {
            if (&q == out && i > 1) {
              cout << "      t = " << t << ": " << q.at(i - 1) << ";" << endl;
            }
            if (&q == out && i == 1) {
              cout << "      t = " << t << ": none;" << endl;
            }
          }

          for (int j = 0; j < t.in().size(); ++j) {
            if (&q == t.in()[j]) {
              cout << "      (t = " << t << " & " << q.size_name() << " = "
                   << q.capacity() - i;
              for (auto &c : t.cond()) {
                cout << " & " << c;
              }
              cout << "): " << t.var_in()[j] << ";" << endl;
            }
          }
        }
        cout << "      TRUE: " << q.at(i) << ";" << endl;
        cout << "    esac;" << endl << endl;
      }
    }
  }

  cout << endl;

//   cout << "CTLSPEC AG !(cc1_q[1] = token\
// & cc1_q[2] = token\
// & cc1_q[3] = token\
// & cc1_qn = 3\
// & cc2_q[1] = token\
// & cc2_q[2] = token\
// & cc2_q[3] = token\
// & cc2_qn = 3\
// & cc3_q[1] = token\
// & cc3_q[2] = token\
// & cc3_q[3] = token\
// & cc3_qn = 3\
// & cc4_q[1] = token\
// & cc4_q[2] = token\
// & cc4_q[3] = token\
// & cc4_qn = 3\
// & cq1[1] = none\
// & cq1[2] = none\
// & cq1n = 0\
// & cq2[1] = token\
// & cq2[2] = token\
// & cq2n = 2\
// & cq3[1] = token\
// & cq3[2] = token\
// & cq3n = 2\
// & cq4[1] = none\
// & cq4[2] = token\
// & cq4n = 1\
// & cx1[1] = none\
// & cx1n = 0\
// & cx2[1] = token\
// & cx2n = 1\
// & cx3[1] = token\
// & cx3n = 1\
// & cx4[1] = none\
// & cx4n = 0\
// & iq1[1] = req\
// & iq1[2] = req\
// & iq1n = 2\
// & iq2[1] = none\
// & iq2[2] = none\
// & iq2n = 0\
// & iq3[1] = none\
// & iq3[2] = none\
// & iq3n = 0\
// & iq4[1] = req\
// & iq4[2] = req\
// & iq4n = 2\
// & dx1[1] = req\
// & dx1n = 1\
// & dx2[1] = none\
// & dx2n = 0)" << endl;

  cout << "CTLSPEC AG !(iq2n = 2 & cc2_qn = 1 & cq2n = 0 & cx2n = 0)" << endl;
  cout << "CTLSPEC AG !(iq2n = 2 & cc2_qn = 2 & cq2n = 0 & cx2n = 0)" << endl;

  return 0;
}
