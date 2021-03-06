
#pragma once

#include <vector>

using namespace std;

namespace Core
{

template<typename T>
class TVector
{
protected:

	typedef vector<T*>	TVec;

	bool				_autoDelete;
	TVec				_data;

public:

	TVector(bool autoDelete=true) : _autoDelete(autoDelete)				{}
	~TVector()															{ clear(); }

	void				clear()
	{
		if(_autoDelete)
		{
            for(auto const& i : _data)
                delete i;
		}

		_data.clear();
	}

	vector<T*>&			getVector()										{ return _data; }
	const vector<T*>&	getVector() const								{ return _data; }
};

};
