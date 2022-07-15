#ifdef _EDITORBUILD
#ifndef  _IMGUIEDITOR

#ifndef RUNTIMESERVER_H

namespace ButiEngine {

class IRuntimeServer {
public:
	virtual std::int32_t Start()=0;
	virtual void Stop() = 0;
};

Value_ptr<IRuntimeServer> CreateRuntimeServer();

}


#endif // !RUNTIMESERVER_H

#endif // ! _IMGUIEDITOR


#endif
