#include "FlumenEngine/Render/FilterManager.hpp"
#include "FlumenEngine/Render/Filter.hpp"
#include "FlumenEngine/Render/RenderManager.hpp"

#define MAXIMUM_FILTER_COUNT 32

Map <Filter*, Word> FilterManager::filters_ = Map <Filter*, Word> (MAXIMUM_FILTER_COUNT);

Map <Filter*, Word> & FilterManager::GetFilters()
{
	return filters_;
}

Filter* FilterManager::GetFilter(const char* name)
{
	return *filters_.Get(name);
}

Filter* FilterManager::AddFilter(const char* name, Filter* filter)
{
	auto filterPointer = filters_.Add(name);
	if(!filterPointer)
		return nullptr;

	*filterPointer = filter;

	return filter;
}

void FilterManager::Update()
{
	auto camera = RenderManager::GetInterfaceCamera();

	for(auto filterIterator = filters_.GetStart(); filterIterator != filters_.GetEnd(); ++filterIterator)
	{
		auto filter = *filterIterator;
		if(!filter)
			continue;

		filter->Update(camera);
	}
}
